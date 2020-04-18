#include "audiohandler.h"
#include "Exceptions/exception.h"
#include "utils/log.h"
#include <string.h>
#include "AudioFile.h"
#include <math.h>
#include "utils/timehandler.h"

#define DEFAULT_BATCH_SIZE 1024
#define DEFAULT_OUTPUT_BUFFER_SIZE 10

using namespace std;
using namespace CS;

jack_port_t * stereoOutputPort[2];

unsigned int batchSize = DEFAULT_BATCH_SIZE;
unsigned long outputBufferSize = DEFAULT_OUTPUT_BUFFER_SIZE * DEFAULT_BATCH_SIZE;

unsigned long batchIndex = 0;
OutputBuffer outputBuffer;

AudioHandler* AudioHandler::instance = nullptr;

AudioHandler* AudioHandler::getInstance()
{
    if ( !instance ) instance = new AudioHandler();
    return instance;
}

AudioHandler::AudioHandler()
{

}

AudioHandler::~AudioHandler()
{
    jack_client_close( _pJackClient );
}

int process(jack_nframes_t nframes, void *arg)
{
    // Increase time
    TimeHandler::getInstance()->increaseTick(batchSize);

    jack_default_audio_sample_t *out;

    // Copy stereo outputBuffer to memory
    out = (jack_default_audio_sample_t*)jack_port_get_buffer (stereoOutputPort[BUFFER_RIGHT], nframes);

    memcpy(
           out,
           &outputBuffer[BUFFER_RIGHT][batchIndex * batchSize],
           sizeof (jack_default_audio_sample_t) * nframes
        );

    out = (jack_default_audio_sample_t*)jack_port_get_buffer (stereoOutputPort[BUFFER_LEFT], nframes);

    memcpy(
            out,
            &outputBuffer[BUFFER_LEFT][batchIndex * batchSize],
            sizeof (jack_default_audio_sample_t) * nframes
        );

    batchIndex++;

    // If its going out of buffer size, go to first batch again and
    // call processOverflow to fill outputBuffer with new data.

    if (batchIndex *batchSize >= outputBufferSize) {
        AudioHandler *handler = (AudioHandler*)arg;
        handler->outputBufferOverflow();
        batchIndex = 0;
    }

    return 0;
}

void AudioHandler::outputBufferOverflow()
{
    outputBuffer.reset();

    auto i = _buffersList.begin();

    while (i != _buffersList.end())
    {
        if ( outputBuffer.play( *i ) )
        {
            i++;
        }
        else
        {
            i = _buffersList.erase(i);
        }
    }
}

void shutdown(void * arg)
{

}

bool AudioHandler::init()
{    
    outputBuffer.setSize(outputBufferSize);

    jack_options_t options = JackNullOption;
    jack_status_t status;

    // Open client conneted to jack server

    _pJackClient = jack_client_open(
                _sJackClientName,
                options,
                &status,
                _sJackServerName
                );

    if ( _pJackClient == nullptr )
    {
        std::string log = "Error creating jack client. ";
        if (status & JackServerFailed)
            log.append("Status: JackServeFailed");

        Log::getInstance().write( log, Log::error_t );

        return false;
    }
    if (status & JackServerStarted)
        Log::getInstance().write( "JACK server started. ", Log::info_t );

    if (status & JackNameNotUnique)
    {
        _sJackClientName = jack_get_client_name( _pJackClient );
        Log::getInstance().write( "Unique name asigned. ", Log::info_t );
    }

    // Set client callbacks

    jack_set_process_callback (_pJackClient, process, this);

    jack_on_shutdown (_pJackClient, shutdown, 0);

    printf ("engine sample rate: %" PRIu32 "\n",
                jack_get_sample_rate (_pJackClient));

    // Crete ports
    stereoOutputPort[0] = jack_port_register (_pJackClient, "output",
                                          JACK_DEFAULT_AUDIO_TYPE,
                                          JackPortIsOutput, 0);

    stereoOutputPort[1] = jack_port_register (_pJackClient, "output2",
                                          JACK_DEFAULT_AUDIO_TYPE,
                                          JackPortIsOutput, 0);
    if (stereoOutputPort[0] == NULL)
    {
        Log::getInstance().write( "no more JACK ports available ", Log::error_t );
        return false;
    }
    if (stereoOutputPort[1] == NULL)
    {
        Log::getInstance().write( "no more JACK ports davailable ", Log::error_t );
        return false;
    }
    // Activate client
    if (jack_activate (_pJackClient))
    {
        Log::getInstance().write( "cannot activate client ", Log::error_t );
        return false;
    }

    TimeHandler::getInstance()->configure(jack_get_sample_rate (_pJackClient),jack_get_buffer_size(_pJackClient));

    const char ** ports = jack_get_ports (_pJackClient, NULL, NULL,
                            JackPortIsPhysical|JackPortIsInput);
    if (ports == NULL)
    {
        Log::getInstance().write( "no physical playback ports", Log::error_t );
        return false;
    }

    if (jack_connect (_pJackClient, jack_port_name (stereoOutputPort[0]), ports[0]))
    {
        Log::getInstance().write( "cannot connect output ports", Log::error_t );
    }
    if (jack_connect (_pJackClient, jack_port_name (stereoOutputPort[1]), ports[1]))
    {
        Log::getInstance().write( "cannot connect output portsd", Log::error_t );
    }

    free (ports);


    // EXAMPLE MUSIC
    AudioFile<float> audioFile;

    audioFile.load ("/home/antiloope/Downloads/Casio-MT-45-16-Beat.wav");
    audioFile.printSummary();

    Sound * tmp = new Sound((AudioFile<float>::AudioBuffer)audioFile.samples,NOW);
    _buffersList.push_back(tmp);

    AudioFile<float>::AudioBuffer buffer;
    buffer.resize(2);
    buffer[0].resize (outputBufferSize*10);
    buffer[1].resize (outputBufferSize*10);

    int numChannels = 1;
    int numSamplesPerChannel = outputBufferSize*10;
    float sampleRate = 44100.f;
    float frequency = 440.f;

    for (int i = 0; i < numSamplesPerChannel; i++)
    {
        //frequency += 0.003;
        float sample = sinf (2. * M_PI * ((float) i / sampleRate) * frequency) ;

        for (int channel = 0; channel < numChannels; channel++)
             buffer[channel][i] = sample * 0.5;
    }

    tmp = new Sound(buffer,2000);
    _buffersList.push_back(tmp);

    outputBufferOverflow();

    return true;
}
