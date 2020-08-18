#include "executor.h"
#include "utils/timehandler.h"
#include "utils/Exceptions/exception.h"
#include "utils/log.h"
#include <cstring>
#include <jack/jack.h>
#include <thread>
#include <queue>

#include <math.h>

using namespace CS;

///////////////////////////////////////////////////
///
///   Definitions and parametrization
///
///////////////////////////////////////////////////

enum ChannelMode {
    mono = 1,
    stereo = 2
};
const char DualBufferMode = 2;
enum DualBuffer {
    first = 0,
    second = 1
} currentBuffer = first;

const size_t DefaultBatchSize = 1024;
const size_t DefaultOutputBufferSize = 10;
const string ClientName = "ChordScript";
const string ServerName = "ChordScriptServer";

const char numberOfSounds = (char)1000;

size_t batchSize = DefaultBatchSize;
size_t outputBufferSize = DefaultOutputBufferSize * DefaultBatchSize;
size_t batchIndex = 0;

///////////////////////////////////////////////////
///
///   Jack and buffers handling
///
///////////////////////////////////////////////////

jack_client_t* jackClient = nullptr;
// Output port to send data
jack_port_t* stereoOutputPort[ChannelMode::stereo];
// Internal usage outputBuffer array to manage dual buffer
OutputBuffer outputBuffer[DualBufferMode];
// Queue to manage insertions in soundsList
queue<Sound*> soundsToAdd;
// Thread to manage soundsList insertions from queue soundsToAdd and to play soundson outputBuffer
thread bufferHandlerThread;
atomic_flag lockBuffer = ATOMIC_FLAG_INIT;

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
        currentBuffer == DualBuffer::first ? currentBuffer = DualBuffer::second : currentBuffer = DualBuffer::first;
        batchIndex = 0;
    }

    if ( batchIndex == 1 ) {
        lockBuffer.clear();
    }

    return 0;
}

void processShutdown(void*) {
}

/**
 * @brief This function is executed in bufferHandlerThread.
 * This function, play sounds in soundsList on secondary buffer and
 * load soundsList with sounds from soundsToAdd queue.
 * @param soundsList A pointer to a list of sounds to be played
 */
void loadBuffer(list<Sound*>* soundsList) {
    while (true) {
        while (lockBuffer.test_and_set())
        {
            // While waiting, load sounds from soundsToAdd to soundsList
            if( !soundsToAdd.empty() )
            {
                soundsList->push_back(soundsToAdd.front());
                soundsToAdd.pop();
            }
        }
        DualBuffer nextBuffer;

        // Switch buffer
        currentBuffer == DualBuffer::first ? nextBuffer = DualBuffer::second : nextBuffer = DualBuffer::first;
        outputBuffer[nextBuffer].reset();

        auto i = soundsList->begin();

        while (i != soundsList->end())
        {
            if ( outputBuffer[nextBuffer].play( *i) )
                i++;
            else
                i = soundsList->erase(i);
        }
        lockBuffer.test_and_set();
    }
}

///////////////////////////////////////////////////
///
///   Executor class
///
///////////////////////////////////////////////////

Executor* Executor::_instance = nullptr;

Executor* Executor::getInstance() {
    if ( !_instance ) _instance = new Executor();
    return _instance;
}

Executor::Executor() {
    for( char i = 0; i < numberOfSounds; i++)
    {
        _availableSounds.push(i);
    }
}

void Executor::addSound(Sound* newSound) const {
    soundsToAdd.push(newSound);
}

unsigned int Executor::getSampleRate() const {
    return jack_get_sample_rate (jackClient);
}

char Executor::getSoundId() {
    char ret = _availableSounds.top();
    return ret;
}

Executor::~Executor() {
    while(Sound* tmp = _soundsList.back()){
        _soundsList.pop_back();
        delete tmp;
    }
    while(Sound* tmp = soundsToAdd.front()){
        soundsToAdd.pop();
        delete tmp;
    }
    jack_client_close(jackClient);
}

void Executor::init() {
    // Set output buffer size based on default outputBufferSize
    outputBuffer[Channel::right].setSize(outputBufferSize);
    outputBuffer[Channel::left].setSize(outputBufferSize);

    // Start the trhead
    bufferHandlerThread = thread(loadBuffer,&_soundsList);

    // Following lines start jack client and connect to the jack server
    jack_options_t options = JackNullOption;
    jack_status_t status;

    string jackClientName = ClientName;
    string jackServerName = ServerName;

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

    // TODO: Think what to send to processShutdown
    jack_on_shutdown (jackClient, processShutdown, 0);

    // TODO: Store sample rate
    // jack_get_sample_rate (jackClient);

    // Crete ports
    stereoOutputPort[Channel::left] = jack_port_register (jackClient, "LeftOutput",
                                          JACK_DEFAULT_AUDIO_TYPE,
                                          JackPortIsOutput, 0);

    stereoOutputPort[Channel::right] = jack_port_register (jackClient, "RightOutput",
                                          JACK_DEFAULT_AUDIO_TYPE,
                                          JackPortIsOutput, 0);
    if (stereoOutputPort[Channel::left] == NULL)
        throw new LogException("No more JACK ports available");
    if (stereoOutputPort[Channel::left] == NULL)
        throw new LogException("No more JACK ports available");

    // Activate client
    if (jack_activate (jackClient))
        throw new LogException("Cannot activate client");

    // Set conficuration for TimeHandler
    TimeHandler::getInstance()->configure(
                jack_get_sample_rate (jackClient),
                jack_get_buffer_size(jackClient)
                );

    // Connect ports to outputPort strucure used to load buffers
    const char ** ports = jack_get_ports (
                jackClient,
                NULL,
                NULL,
                JackPortIsPhysical|JackPortIsInput
            );

    if (ports == NULL)
        throw new LogException("No physical playback ports");

    if (jack_connect (jackClient, jack_port_name (stereoOutputPort[Channel::left]), ports[Channel::left]) ||
        jack_connect (jackClient, jack_port_name (stereoOutputPort[Channel::right]), ports[Channel::right]))
        throw new LogException("Cannot connect output ports");

    free (ports);

    Log::getInstance().write("AudioServerAdapter correctly initilized",Log::info_t);

//    AudioFile<float>::AudioBuffer buffer;
//    buffer.resize(2);
//    buffer[0].resize (outputBufferSize*10);
//    buffer[1].resize (outputBufferSize*10);

//    int numChannels = 2;
//    int numSamplesPerChannel = outputBufferSize*10;
//    float sampleRate = 44100.f;
//    float frequency = 440.f;

//    for (int i = 0; i < numSamplesPerChannel; i++)
//    {
//        frequency += 0.003;
//        float sample = sinf (2. * M_PI * ((float) i / sampleRate) * frequency) ;

//        for (int channel = 0; channel < numChannels; channel++)
//             buffer[channel][i] = sample * 0.5;
//    }

//    Sound * tmp = new Sound(buffer,NOW);
//    this->addSound(tmp);
//    tmp = new Sound(buffer,20);
//    this->addSound(tmp);
//    tmp = new Sound(buffer,10);
//    this->addSound(tmp);
}
