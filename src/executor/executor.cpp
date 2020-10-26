#include "executor.h"
#include "utils/timehandler.h"
#include "utils/Exceptions/exception.h"
#include "playables/playable.h"
#include "utils/log.h"
#include <cstring>
#include <thread>
#include <mutex>
#include <queue>
#include <cstring>
#include <jack/jack.h>
#include <jack/control.h>
#include <ctime>
#include <future>

using namespace CS;
using namespace std;

/**
 * @brief The ChannelMode enum
 * Used to define buffer sizes to mono or stereo
 */
enum ChannelMode {
    Mono = 1,
    Stereo = 2
};

/**
 * @brief The DualBuffer enum is used to choose a buffer
 * in a dual buffer
 */
enum DualBuffer {
    First = 0,
    Second = 1
} currentBuffer = First;

/**
 * @brief DUAL_BUFFER_MODE constant used to define
 * dual buffer mode
 */
const char DUAL_BUFFER_MODE = 2;

/**
 * @brief DEFAULT_BATCH_SIZE is the number of samples per batch
 * to store when client callback is called
 */
const size_t DEFAULT_BATCH_SIZE = 1024;
/**
 * @brief DEFAULT_OUTPUT_BUFFER_SIZE is the number of batches stored
 * on each buffer channel
 */
const size_t DEFAULT_OUTPUT_BUFFER_SIZE = 10;
/**
 * @brief DEFAULT_SAMPLE_RATE is the recommended sample rate
 * in order to use wav samples on original sample rate.
 */
const size_t DEFAULT_SAMPLE_RATE = 48000;

const string CLIENT_NAME = "ChordScript";
const string SERVER_NAME = "JACK Input Client";

/**
 * @brief MAX_NUMBER_OF_SOUNDS the max number of sounds that could be
 * queued at the same time
 */
const char MAX_NUMBER_OF_SOUNDS = (char)2000;

/** Non const values used in the rest of the code**/
size_t sampleRate = DEFAULT_SAMPLE_RATE;
size_t batchSize = DEFAULT_BATCH_SIZE;
size_t outputBufferSize = DEFAULT_OUTPUT_BUFFER_SIZE * DEFAULT_BATCH_SIZE;
/**
 * @brief batchIndex is the index to iterate the buffer on each
 * callback call
 */
size_t batchIndex = 0;

/**
 * @brief stereoOutputPort
 */
jack_port_t* stereoOutputPort[ChannelMode::Stereo];
jackctl_server_t* jackServer = nullptr;
jack_client_t* jackClient = nullptr;

// Internal usage outputBuffer array to manage dual buffer
OutputBuffer outputBuffer[DUAL_BUFFER_MODE];
// Queue to manage insertions in soundsList
queue<tuple<Playable*,string>> soundsToAdd;
// Queue to manage removes in soundsList
queue<tuple<Playable*,string>> soundsToRemove;
// Thread to manage soundsList insertions from queue soundsToAdd and to play soundson outputBuffer
thread bufferHandlerThread;
atomic_flag lockBuffer = ATOMIC_FLAG_INIT;
mutex mutexQueues;

bool removeAll = false;

atomic_flag isRunning = ATOMIC_FLAG_INIT;
mutex recordingFile;

///////////////////////////////////////////////////
///
///   Jack callbacks
///
///////////////////////////////////////////////////

int processCallback(jack_nframes_t nframes,void*) {
    // Increase time
    TimeHandler::getInstance()->increaseTick(batchSize);

    jack_default_audio_sample_t *out;

    // Copy stereo outputBuffer to memory
    out = (jack_default_audio_sample_t*)
        jack_port_get_buffer(
            stereoOutputPort[Channel::Right],
            nframes );

    memcpy(
        out,
        &outputBuffer[currentBuffer][Channel::Right][batchIndex * batchSize],
        sizeof (jack_default_audio_sample_t) * nframes );

    out = (jack_default_audio_sample_t*)
        jack_port_get_buffer (
            stereoOutputPort[Channel::Left],
            nframes );

    memcpy(
        out,
        &outputBuffer[currentBuffer][Channel::Left][batchIndex * batchSize],
        sizeof (jack_default_audio_sample_t) * nframes );

    batchIndex++;

    // If its going out of buffer size, go to first batch again and
    // call processOverflow to fill outputBuffer with new data.

    if( batchIndex * batchSize >= outputBufferSize )
    {
        currentBuffer == DualBuffer::First ? currentBuffer = DualBuffer::Second : currentBuffer = DualBuffer::First;
        batchIndex = 0;
    }

    if ( batchIndex == 1 )
        lockBuffer.clear();

    return 0;
}

void processShutdown(void*) {}

/**
 * @brief This function is executed in bufferHandlerThread.
 * This function, play sounds in soundsList on secondary buffer and
 * load soundsList with sounds from soundsToAdd queue.
 * @param soundsList A pointer to a list of sounds to be played
 */
void Executor::loadBuffer(list<tuple<Playable*,string>>* soundsList) {
    isRunning.test_and_set();
    while( isRunning.test_and_set() )
    {
        while( lockBuffer.test_and_set() )
        {
            if( mutexQueues.try_lock() )
            {
                // While waiting, load sounds from soundsToAdd to soundsList
                if( !soundsToAdd.empty() )
                {
                    soundsList->push_back(soundsToAdd.front());
                    soundsToAdd.pop();
                }

                while( !soundsToRemove.empty() )
                {
                    auto it = soundsList->begin();
                    while( it != soundsList->end() )
                    {
                        if( get<1>(*it) == get<1>(soundsToRemove.front()) )
                        {
                            auto tmp = get<0>(*it);
                            it = soundsList->erase(it);
                            delete tmp;
                        }
                        else
                            it++;
                    }
                    soundsToRemove.pop();
                }

                if( removeAll )
                {
                    for(auto&& i : *soundsList)
                        delete get<0>(i);

                    soundsList->clear();
                    removeAll = false;
                }

                mutexQueues.unlock();
            }
        }
        DualBuffer nextBuffer;

        // Switch buffer
        currentBuffer == DualBuffer::First ? nextBuffer = DualBuffer::Second : nextBuffer = DualBuffer::First;
        outputBuffer[nextBuffer].reset();

        auto i = soundsList->begin();

        while( i != soundsList->end() )
        {
            if( outputBuffer[nextBuffer].play(get<0>(*i)) )
                i++;
            else
            {
                Playable* tmp = get<0>(*i);
                delete tmp;
                i = soundsList->erase(i);
            }
        }

        if( recordingFile.try_lock() )
        {
            Executor::getInstance()->addToRecord();
            recordingFile.unlock();
        }

        lockBuffer.test_and_set();
    }
}

bool serverDeviceAcquire(const char *) {
    return true;
}

void serverDeviceRelease(const char *) {
}

///////////////////////////////////////////////////
///
///   Executor class
///
///////////////////////////////////////////////////

Executor* Executor::_instance = nullptr;

Executor* Executor::getInstance() {
    if ( !_instance )
        _instance = new Executor();

    return _instance;
}

Executor::Executor() {
    for( char i = 0; i < MAX_NUMBER_OF_SOUNDS; i++)
        _availableSounds.push(i);
}

void Executor::startRecording() {
    recordingFile.unlock();
}

void Executor::stopRecording() {
    recordingFile.lock();

    try
    {
        time_t now = time(0);
        char stringTime[17];
        strftime(stringTime,20,"%g_%m_%d_%H_%M_%S",localtime(&now));
        string fileName = "records/rec_";
        fileName.append(stringTime);
        fileName += ".wav";
        _record.save(fileName);
    }
    catch (exception &e)
    {
        Log::getInstance().write(e.what(),Log::logType::error_t);
    }

}

void Executor::addSound(Playable* newSound, string variableName) const {
    mutexQueues.lock();
    soundsToAdd.push({newSound,variableName});
    mutexQueues.unlock();
}

void Executor::removeSound(Playable* sound, string variableName) const {
    mutexQueues.lock();
    soundsToRemove.push({sound,variableName});
    mutexQueues.unlock();
}

void Executor::removeAllSounds() const {
    mutexQueues.lock();
    removeAll = true;
    mutexQueues.unlock();
}

void Executor::addToRecord() {
    _record.samples[Channel::Right].insert(
        _record.samples[Channel::Right].end(),
        outputBuffer[currentBuffer][Channel::Right].begin(),
        outputBuffer[currentBuffer][Channel::Right].end());

    _record.samples[Channel::Left].insert(
        _record.samples[Channel::Left].end(),
        outputBuffer[currentBuffer][Channel::Left].begin(),
        outputBuffer[currentBuffer][Channel::Left].end());
}

unsigned int Executor::getSampleRate() const {
    return jack_get_sample_rate (jackClient);
}

char Executor::getSoundId() {
    char ret = _availableSounds.top();
    return ret;
}

Executor::~Executor() {
    while( !_soundsList.empty() )
    {
        auto tmp = get<0>(_soundsList.front());
        _soundsList.pop_front();
        delete tmp;
    }
    while( !soundsToAdd.empty() )
    {
        auto tmp = get<0>(soundsToAdd.front());
        soundsToAdd.pop();
        delete tmp;
    }
    jack_client_close(jackClient);
    jackctl_server_destroy(jackServer);
}

void Executor::clientInit() {
    jack_options_t options = JackNullOption;
    jack_status_t status;

    string jackClientName = CLIENT_NAME;
    string jackServerName = SERVER_NAME;

    // Create jack client connected to server
    jackClient = jack_client_open(
        jackClientName.c_str(),
        options,
        &status,
        jackServerName.c_str() );
    if ( jackClient == nullptr )
    {
        string log = "Error creating jack client. ";
        if (status & JackServerFailed) log.append("Status: JackServerFailed");
        throw new LogException(log);
    }
    if (status & JackServerStarted)
        Log::getInstance().write( "JACK server started. ", Log::info_t );

    if (status & JackNameNotUnique)
    {
        jackClientName = jack_get_client_name( jackClient );
        Log::getInstance().write( "Unique name asigned. ", Log::info_t );
    }

    // Set client callbacks

    jack_set_process_callback (jackClient, processCallback, nullptr);

    jack_on_shutdown (jackClient, processShutdown, 0);

    // Crete ports
    stereoOutputPort[Channel::Left] = jack_port_register (jackClient, "LeftOutput",
                                                         JACK_DEFAULT_AUDIO_TYPE,
                                                         JackPortIsOutput, 0);

    stereoOutputPort[Channel::Right] = jack_port_register (jackClient, "RightOutput",
                                                          JACK_DEFAULT_AUDIO_TYPE,
                                                          JackPortIsOutput, 0);
    if (stereoOutputPort[Channel::Left] == NULL)
        throw new LogException("No more JACK ports available");
    if (stereoOutputPort[Channel::Right] == NULL)
        throw new LogException("No more JACK ports available");

    // Activate client
    if (jack_activate (jackClient))
        throw new LogException("Cannot activate client");

    // Connect ports to outputPort strucure used to load buffers
    const char ** ports = jack_get_ports (
        jackClient,
        NULL,
        NULL,
        JackPortIsPhysical|JackPortIsInput
        );

    if (ports == NULL)
        throw new LogException("No physical playback ports");

    if (jack_connect (jackClient, jack_port_name (stereoOutputPort[Channel::Left]), ports[Channel::Right]) ||
        jack_connect (jackClient, jack_port_name (stereoOutputPort[Channel::Right]), ports[Channel::Left]))
        throw new LogException("Cannot connect output ports");

    free (ports);

    Log::getInstance().write("Jack client connected.",Log::info_t);
}

bool Executor::serverInit() {
    // Creating server
    jackServer = jackctl_server_create(
                        serverDeviceAcquire,
                        serverDeviceRelease);

    if( !jackServer )
        throw new LogException("Error creating jack server.");

    // Choosing appropiate driver
    const _JSList* drivers = jackctl_server_get_drivers_list(jackServer);

    const _JSList* driver = drivers;
    while( driver )
    {
        const char* name = jackctl_driver_get_name((jackctl_driver_t*)(driver->data));

        if( strcmp(name, "alsa") != 0 && strcmp(name, "portaudio") != 0 && driver->next )
            driver = driver->next;
        else
            break;
    }

    // Setting parameters
    const _JSList* parameters = jackctl_driver_get_parameters((jackctl_driver_t*)(driver->data));

    while( parameters )
    {
        const char* name = jackctl_parameter_get_name((jackctl_parameter_t*)(parameters->data));
        if( !strcmp(name,"rate") )
        {
            jackctl_parameter_value* rate = new jackctl_parameter_value();
            rate->ui = 48000;
            if( !jackctl_parameter_set_value((jackctl_parameter_t*)(parameters->data),rate) )
                throw new LogException("Error setting parameter rate in jack server.");
        }
        else if( !strcmp(name,"period") )
        {
            jackctl_parameter_value* period = new jackctl_parameter_value();
            period->ui = 1024;
            if( !jackctl_parameter_set_value((jackctl_parameter_t*)(parameters->data),period) )
                throw new LogException("Error setting parameter period in jack server.");
        }
        else if( !strcmp(name,"nperiods") )
        {
            jackctl_parameter_value* periods = new jackctl_parameter_value();
            periods->ui = 10;
            if( !jackctl_parameter_set_value((jackctl_parameter_t*)(parameters->data),periods) )
                throw new LogException("Error setting parameter nperiods in jack server.");
        }

        parameters = parameters->next;
    }

    // Opening and starting server
    if( !jackctl_server_open(jackServer,(jackctl_driver_t*)(driver->data)) )
        return false;

    if( !jackctl_server_start(jackServer) )
        return false;

    Log::getInstance().write("Jack server started.",Log::info_t);

    return true;
}

void Executor::closeAll() {
    // Close client and server
    jack_client_close(jackClient);
    jackClient = nullptr;
    jackctl_server_destroy(jackServer);
    jackServer = nullptr;

    // Join thread
    isRunning.clear();
    lockBuffer.clear();
    bufferHandlerThread.join();
}

bool Executor::isServerRunning() {
    return _isServerRunning;
}

void Executor::startServer() {
    _isServerRunning = serverInit();
    clientInit();
}

void Executor::restartServer() {
    killServer();
    startServer();
}

void Executor::killServer() {
    jack_deactivate(jackClient);
    jack_client_close(jackClient);
    jackctl_server_stop(jackServer);
    jackctl_server_close(jackServer);
    jackctl_server_destroy(jackServer);

    _isServerRunning = false;

    Log::getInstance().write("Server killed",Log::logType::info_t);
}

void Executor::init() {
    // Set output buffer size based on default outputBufferSize
    outputBuffer[Channel::Right].setSize(outputBufferSize);
    outputBuffer[Channel::Left].setSize(outputBufferSize);

    recordingFile.lock();

    // Start the trhead
    bufferHandlerThread = thread(loadBuffer,&_soundsList);

    serverInit();
    _isServerRunning = true;
    // Set conficuration for TimeHandler
    TimeHandler::getInstance()->configure(
        sampleRate,
        DEFAULT_BATCH_SIZE
        );

    clientInit();

    _record.setSampleRate(sampleRate);
    _record.setAudioBufferSize(ChannelMode::Stereo,0);
}
