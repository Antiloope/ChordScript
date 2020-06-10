#include "audioserveradapter.h"
#include "utils/timehandler.h"
#include "utils/Exceptions/exception.h"
#include "utils/log.h"
#include <cstring>
#include <jack/jack.h>
#include <thread>
#include <atomic>

#include <math.h>// TODO: REMOVE

using namespace CS;

constexpr unsigned int DEFAULT_BATCH_SIZE = 1024;
constexpr unsigned int DEFAULT_OUTPUT_BUFFER_SIZE = 10;
constexpr unsigned short LEFT = 0;
constexpr unsigned short RIGHT = 1;
constexpr unsigned short STEREO = 2;

size_t batchSize = DEFAULT_BATCH_SIZE;
size_t outputBufferSize = DEFAULT_OUTPUT_BUFFER_SIZE * DEFAULT_BATCH_SIZE;
size_t batchIndex = 0;

jack_client_t* jackClient = nullptr;
jack_port_t* stereoOutputPort[STEREO];
OutputBuffer outputBuffer[2];
unsigned short currentBuffer = 0;

thread t;
atomic_flag lockBuffer = ATOMIC_FLAG_INIT;

AudioServerAdapter* AudioServerAdapter::_instance = nullptr;

AudioServerAdapter* AudioServerAdapter::getInstance(){
    if ( !_instance ) _instance = new AudioServerAdapter();
    return _instance;
}

int processCallback(jack_nframes_t nframes,void* arg){
    // Increase time
    TimeHandler::getInstance()->increaseTick(batchSize);

    jack_default_audio_sample_t *out;

    // Copy stereo outputBuffer to memory
    out = (jack_default_audio_sample_t*)jack_port_get_buffer (
                stereoOutputPort[RIGHT],
                nframes
                );

    memcpy(
           out,
           &outputBuffer[currentBuffer][RIGHT][batchIndex * batchSize],
           sizeof (jack_default_audio_sample_t) * nframes
        );

    out = (jack_default_audio_sample_t*)jack_port_get_buffer (
                stereoOutputPort[LEFT],
                nframes
                );

    memcpy(
            out,
            &outputBuffer[currentBuffer][LEFT][batchIndex * batchSize],
            sizeof (jack_default_audio_sample_t) * nframes
        );

    batchIndex++;

    // If its going out of buffer size, go to first batch again and
    // call processOverflow to fill outputBuffer with new data.

    if (batchIndex * batchSize >= outputBufferSize){
        currentBuffer == 0 ? currentBuffer = 1 : currentBuffer = 0;
        batchIndex = 0;
    }

    if (batchIndex == 1) {
        lockBuffer.clear();
    }

    return 0;
}

void loadBuffer(list<Sound*>* soundsList){
    while (1) {
        while (lockBuffer.test_and_set()) {}
        unsigned short nextBuffer;

        //Separate process in two steps.
        // One to create a buffer
        // And another to set this buffer to current outputBuffer
        currentBuffer == 0 ? nextBuffer = 1 : nextBuffer = 0;
        outputBuffer[nextBuffer].reset();

        auto i = soundsList->begin();

        while (i != soundsList->end())
        {
            if ( outputBuffer[nextBuffer].play( *i ) )
            {
                i++;
            }
            else
            {
                i = soundsList->erase(i);
            }
        }
        lockBuffer.test_and_set();
    }
}

void processShutdown(void* arg){
}

AudioServerAdapter::AudioServerAdapter() {

}

AudioServerAdapter::~AudioServerAdapter() {
    while(Sound* tmp = _soundsList.back()){
        _soundsList.pop_back();
        delete tmp;
    }
    jack_client_close(jackClient);
}

void AudioServerAdapter::init() {
    outputBuffer[0].setSize(outputBufferSize);
    outputBuffer[1].setSize(outputBufferSize);

    t = thread(loadBuffer,&_soundsList);

    jack_options_t options = JackNullOption;
    jack_status_t status;

    string jackClientName = "simple"; // TODO: Set client name
    string jackServerName = "asd";  // TODO: Set server name

    jackClient = jack_client_open(
                jackClientName.c_str(),
                options,
                &status,
                jackServerName.c_str()
                );
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

    // TODO: Think what to send to processCalback
    jack_set_process_callback (jackClient, processCallback, this);

    // TODO: Think what to send to processShutdown
    jack_on_shutdown (jackClient, processShutdown, 0);

    // TODO: Store sample rate
    // jack_get_sample_rate (jackClient);

    // Crete ports
    stereoOutputPort[LEFT] = jack_port_register (jackClient, "LeftOutput",
                                          JACK_DEFAULT_AUDIO_TYPE,
                                          JackPortIsOutput, 0);

    stereoOutputPort[RIGHT] = jack_port_register (jackClient, "RightOutput",
                                          JACK_DEFAULT_AUDIO_TYPE,
                                          JackPortIsOutput, 0);
    if (stereoOutputPort[LEFT] == NULL)
        throw new LogException("No more JACK ports available");
    if (stereoOutputPort[LEFT] == NULL)
        throw new LogException("No more JACK ports available");

    // Activate client
    if (jack_activate (jackClient))
        throw new LogException("Cannot activate client");

    TimeHandler::getInstance()->configure(
                jack_get_sample_rate (jackClient),
                jack_get_buffer_size(jackClient)
                );

    const char ** ports = jack_get_ports (
                jackClient,
                NULL,
                NULL,
                JackPortIsPhysical|JackPortIsInput
            );

    if (ports == NULL)
        throw new LogException("No physical playback ports");

    if (jack_connect (jackClient, jack_port_name (stereoOutputPort[LEFT]), ports[LEFT]) ||
        jack_connect (jackClient, jack_port_name (stereoOutputPort[RIGHT]), ports[RIGHT]))
        throw new LogException("Cannot connect output ports");

    free (ports);

    Log::getInstance().write("AudioServerAdapter correctly initilized",Log::info_t);

//    AudioFile<float>::AudioBuffer buffer;
//    buffer.resize(2);
//    buffer[0].resize (outputBufferSize*10);
//    buffer[1].resize (outputBufferSize*10);

//    int numChannels = 1;
//    int numSamplesPerChannel = outputBufferSize*10;
//    float sampleRate = 44100.f;
//    float frequency = 440.f;

//    for (int i = 0; i < numSamplesPerChannel; i++)
//    {
//        //frequency += 0.003;+
//        float sample = sinf (2. * M_PI * ((float) i / sampleRate) * frequency) ;

//        for (int channel = 0; channel < numChannels; channel++)
//             buffer[channel][i] = sample * 0.5;
//    }

//    Sound * tmp = new Sound(buffer,NOW);
//    _soundsList.push_back(tmp);

}
