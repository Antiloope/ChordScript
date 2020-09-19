#include "executor.h"
#include "utils/timehandler.h"
#include "utils/Exceptions/exception.h"
#include "utils/log.h"
#include <cstring>
#include <thread>
#include <mutex>
#include <queue>
#include <cstring>
#include <jack/jack.h>
#include <jack/control.h>
#include <math.h>

using namespace CS;

/**
 * @brief The ChannelMode enum
 * Used to define buffer sizes to mono or stereo
 */
enum ChannelMode {
    Mono = 1,
    Stereo = 2
};

/**
 * @brief DUAL_BUFFER_MODE constant used to define
 * dual buffer mode
 */
const char DUAL_BUFFER_MODE = 2;

/**
 * @brief The DualBuffer enum is used to choose a buffer
 * in a dual buffer
 */
enum DualBuffer {
    First = 0,
    Second = 1
} currentBuffer = First;

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
queue<Playable*> soundsToAdd;
// Queue to manage removes in soundsList
queue<Playable*> soundsToRemove;
// Thread to manage soundsList insertions from queue soundsToAdd and to play soundson outputBuffer
thread bufferHandlerThread;
atomic_flag lockBuffer = ATOMIC_FLAG_INIT;
mutex mutexQueues;

bool removeAll = false;

atomic_flag isRunning = ATOMIC_FLAG_INIT;

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
            stereoOutputPort[Channel::right],
            nframes );

    memcpy(
        out,
        &outputBuffer[currentBuffer][Channel::right][batchIndex * batchSize],
        sizeof (jack_default_audio_sample_t) * nframes );

    out = (jack_default_audio_sample_t*)
        jack_port_get_buffer (
            stereoOutputPort[Channel::left],
            nframes );

    memcpy(
        out,
        &outputBuffer[currentBuffer][Channel::left][batchIndex * batchSize],
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
void loadBuffer(list<Playable*>* soundsList) {
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

                if( !soundsToRemove.empty() )
                {
                    for( Playable* sound : *soundsList )
                    {
                        if( sound == soundsToRemove.front() )
                            delete soundsToRemove.front();
                    }
                    soundsList->remove(soundsToRemove.front());
                    soundsToRemove.pop();
                }

                if( removeAll )
                {
                    for(auto&& i : *soundsList)
                        delete i;

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

        while (i != soundsList->end())
        {
            if( outputBuffer[nextBuffer].play(*i) )
                i++;
            else
            {
                Playable* tmp = *i;
                delete tmp;
                i = soundsList->erase(i);
            }
        }
        lockBuffer.test_and_set();
    }
}

bool serverDeviceAcquire(const char *name) {
    Log::getInstance().write(name,Log::logType::info_t);
    return true;
}

void serverDeviceRelease(const char *) {
    Log::getInstance().write("salienod",Log::logType::info_t);
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

void Executor::addSound(Playable* newSound) const {
    mutexQueues.lock();
    soundsToAdd.push(newSound);
    mutexQueues.unlock();
}

void Executor::removeSound(Playable* sound) const {
    mutexQueues.lock();
    soundsToRemove.push(sound);
    mutexQueues.unlock();
}

void Executor::removeAllSounds() const {
    mutexQueues.lock();
    removeAll = true;
    mutexQueues.unlock();
}

unsigned int Executor::getSampleRate() const {
    return jack_get_sample_rate (jackClient);
}

char Executor::getSoundId() {
    char ret = _availableSounds.top();
    return ret;
}

Executor::~Executor() {
    while(Playable* tmp = _soundsList.back())
    {
        _soundsList.pop_back();
        delete tmp;
    }
    while(Playable* tmp = soundsToAdd.front())
    {
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
    stereoOutputPort[Channel::left] = jack_port_register (jackClient, "LeftOutput",
                                                         JACK_DEFAULT_AUDIO_TYPE,
                                                         JackPortIsOutput, 0);

    stereoOutputPort[Channel::right] = jack_port_register (jackClient, "RightOutput",
                                                          JACK_DEFAULT_AUDIO_TYPE,
                                                          JackPortIsOutput, 0);
    if (stereoOutputPort[Channel::left] == NULL)
        throw new LogException("No more JACK ports available");
    if (stereoOutputPort[Channel::right] == NULL)
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

    if (jack_connect (jackClient, jack_port_name (stereoOutputPort[Channel::left]), ports[Channel::right]) ||
        jack_connect (jackClient, jack_port_name (stereoOutputPort[Channel::right]), ports[Channel::left]))
        throw new LogException("Cannot connect output ports");

    free (ports);
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

void Executor::clientRestart() {
    if( jackClient )
    {
        jack_deactivate(jackClient);

        jack_activate(jackClient);

        // Crete ports
        stereoOutputPort[Channel::left] = jack_port_register (jackClient, "LeftOutput",
                                                             JACK_DEFAULT_AUDIO_TYPE,
                                                             JackPortIsOutput, 0);

        stereoOutputPort[Channel::right] = jack_port_register (jackClient, "RightOutput",
                                                              JACK_DEFAULT_AUDIO_TYPE,
                                                              JackPortIsOutput, 0);
        if (stereoOutputPort[Channel::left] == NULL)
            throw new LogException("No more JACK ports available");
        if (stereoOutputPort[Channel::right] == NULL)
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

        if (jack_connect (jackClient, jack_port_name (stereoOutputPort[Channel::left]), ports[Channel::right]) ||
            jack_connect (jackClient, jack_port_name (stereoOutputPort[Channel::right]), ports[Channel::left]))
            throw new LogException("Cannot connect output ports");

        free (ports);
    }
    else
        clientInit();
}

void Executor::serverRestart() {
    if( jackServer )
        jackctl_server_destroy(jackServer);

    serverInit();
}

void Executor::init() {
    // Set output buffer size based on default outputBufferSize
    outputBuffer[Channel::right].setSize(outputBufferSize);
    outputBuffer[Channel::left].setSize(outputBufferSize);

    // Start the trhead
    bufferHandlerThread = thread(loadBuffer,&_soundsList);

    serverInit();

    // Set conficuration for TimeHandler
    TimeHandler::getInstance()->configure(
        sampleRate,
        DEFAULT_OUTPUT_BUFFER_SIZE
        );

    clientInit();

    Log::getInstance().write("AudioServerAdapter correctly initilized",Log::info_t);
}
