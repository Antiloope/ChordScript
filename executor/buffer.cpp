#include "buffer.h"
#include "executorinterface.h"
#include <math.h>
#include "interpteter/context.h"
#include <cstring>

using namespace CS;

Buffer::Buffer() {
    _buffer = nullptr;
}

Buffer::Buffer(AudioBuffer audioBuffer) {
    _buffer = new AudioBuffer(audioBuffer);
}

Buffer::~Buffer() {
    if( _buffer )
        delete _buffer;
}

AudioBuffer* Buffer::getBuffer() {
    return _buffer;
}

vector<float>& Buffer::operator[](Channel a) {
    return (*_buffer)[a];
}

bool OutputBuffer::play(Playable* sound) {
    if( !sound )
        return false;

    size_t bufferSize = (*_buffer)[Channel::right].size();

    Buffer newBuffer(AudioBuffer(2));
    newBuffer[Channel::right].resize(bufferSize);
    newBuffer[Channel::left].resize(bufferSize);

    sound->play(TimeHandler::getInstance()->getCurrentTick(),newBuffer);

    BufferChannel* rightChannel = &(*_buffer)[Channel::right];
    BufferChannel* leftChannel = &(*_buffer)[Channel::left];

    for( size_t i = 0; i < bufferSize; i++ )
    {
        (*rightChannel)[i] = (*rightChannel)[i] + newBuffer[Channel::right][i];
        (*leftChannel)[i] = (*leftChannel)[i] + newBuffer[Channel::left][i];
    }

    if( sound->isPlayed() )
        return false;

    return true;
}

OutputBuffer::OutputBuffer() {
    _buffer = new AudioBuffer();
    _buffer->resize(2);
}

void OutputBuffer::setSize(unsigned long size) {
    (*_buffer)[0].resize(size);
    (*_buffer)[1].resize(size);
}

OutputBuffer::~OutputBuffer() {}

void OutputBuffer::reset() {
    for(size_t i = 0 ; i < (*_buffer)[Channel::right].size(); i++)
    {
        (*_buffer)[Channel::right][i] = 0.f;
        (*_buffer)[Channel::left][i] = 0.f;
    }
}

Sound::Sound(const Sound& sound) {
    _progress = sound._progress;
    _isPlayed = sound._isPlayed;
    _startTick = sound._startTick;
    _freq = sound._freq;
    _duration = sound._freq;
    _function = sound._function;
    _amplitudeFactor = sound._amplitudeFactor->clone();
    _amplitudeOffset = sound._amplitudeOffset->clone();
    _freqFactor = sound._freqFactor->clone();
    _freqModulation= sound._freqModulation->clone();
    _absoluteFreq = sound._absoluteFreq ? sound._absoluteFreq->clone() :  nullptr;
    _panning = sound._panning->clone();
}

Sound::Sound(double(*function)(double)) {
    _function = function;
    _amplitudeFactor = new ConstModifier(1);
    _amplitudeOffset = new ConstModifier(0);
    _freqFactor = new ConstModifier(1);
    _freqModulation = new ConstModifier(0);
    _panning = new ConstModifier(0.5);
}

Sound::Sound(double(*function)(double), double freq, double duration, tick_t startTick) {
    _function = function;
    _freq = freq;
    _duration = duration;
    _startTick = startTick;
    _amplitudeFactor = new ConstModifier(1);
    _amplitudeOffset = new ConstModifier(0);
    _freqFactor = new ConstModifier(1);
    _freqModulation = new ConstModifier(0);
    _panning = new ConstModifier(0.5);
}

Sound::~Sound() {
    if( _amplitudeFactor ) delete _amplitudeFactor;
    if( _freqFactor ) delete _freqFactor;
    if( _amplitudeOffset ) delete _amplitudeOffset;
    if( _freqModulation ) delete _freqModulation;
    if( _absoluteFreq ) delete _absoluteFreq;
    if( _panning ) delete _panning;
}

bool Playable::isPlayed() const {
    return _isPlayed;
}

void Sound::setAmplitudeFactor(double factor) {
    if( _amplitudeFactor )
        delete _amplitudeFactor;

    _amplitudeFactor = new ConstModifier(factor);
}

void Sound::setAmplitudeFactor(Sound sound) {
    if( _amplitudeFactor )
    {
        if( _amplitudeFactor->setAmplitudeFactor(sound) )
            return;
        else
            delete _amplitudeFactor;
    }

    _amplitudeFactor = new SoundModifier(sound);
}

void Sound::setAmplitudeOffset(double offset) {
    if( _amplitudeOffset )
        delete _amplitudeOffset;

    _amplitudeOffset = new ConstModifier(offset);
}

void Sound::setAmplitudeOffset(Sound sound) {
    if( _amplitudeOffset )
    {
        if( _amplitudeOffset->setAmplitudeOffset(sound) )
            return;
        else
            delete _amplitudeOffset;
    }
    _amplitudeOffset = new SoundModifier(sound);
}

void Sound::setFreqFactor(double factor) {
    if( _freqFactor )
        delete _freqFactor;

    _freqFactor = new ConstModifier(factor);
}

void Sound::setFreqModulation(Sound sound) {
    if( _freqFactor )
        delete _freqFactor;

    _freqFactor = new SoundModifier(sound);
}

void Sound::setAbsoluteFreq(double freq) {
    if( _absoluteFreq )
        delete _absoluteFreq;

    _absoluteFreq = new ConstModifier(freq);
}

void Sound::clearAbsoluteFreq() {
    if( _absoluteFreq )
        delete _absoluteFreq;
}

void Sound::setPanning(double pan) {
    if( _panning )
        delete _panning;

    _panning = new ConstModifier(pan);
}

void Sound::setPanning(Sound sound) {
    if( _panning )
        delete _panning;

    _panning = new SoundModifier(sound);
}

Sound* Sound::generate(double freq, double duration, tick_t startTick) {
    Sound* ret = new Sound(*this);
    ret->load(freq,duration,startTick);
    return ret;
}

void Sound::load(double freq, double duration, tick_t startTick) {
    _freq = freq;
    _duration = duration;
    _startTick = startTick;
    _progress = 0;
    _isPlayed = false;
}

void Sound::play(tick_t currentTick,Buffer& bufferToLoad) {
    static double sampleRate = ExecutorInterface::getSampleRate();

    size_t bufferSize = bufferToLoad[Channel::right].size();

    signed long span = currentTick - _startTick;

    size_t cursor = 0;

    tick_t durationTicks = (tick_t)((double)_duration * (double)ExecutorInterface::getSampleRate());

    if( span <= 0 )
    {
        if( abs(span) >= (signed long)bufferSize )
            return;
        cursor = -span;
    }

    double freq;

    for( ; cursor < bufferSize; cursor++ )
    {
        if( _absoluteFreq )
            freq = _absoluteFreq->getPositiveValue(_freq);
        else
            freq = _freq * _freqFactor->getPositiveValue(_freq);

        double value =
            _function(
                2. * M_PI * freq *
                std::fmod(
                        (double)_progress / sampleRate,
                        1./freq) +
                _freqModulation->getValue(_freq) ) *
                _amplitudeFactor->getPositiveValue(_freq) +
            _amplitudeOffset->getValue(_freq);

        if( _progress >= durationTicks - 1000 )
        {
            double factor = ((double)durationTicks - (double)_progress) / (1000);
            value = value * factor;
        }
        if( _progress < 1000 )
        {
            double factor = ((double)_progress) / (1000.);
            value = value * factor;
        }
        if( _progress >= durationTicks )
        {
            if( abs(value) < 0.001 )
            {
                _isPlayed = true;
                return;
            }
        }
        _progress++;

        bufferToLoad[Channel::right][cursor] = _panning->getPositiveValue(_freq) * value;
        bufferToLoad[Channel::left][cursor] = (1 - _panning->getPositiveValue(_freq)) * value;
    }
}

double Sound::getInstantValue(double freq) {
    static size_t sampleRate = ExecutorInterface::getSampleRate();
    double f;

    if( _absoluteFreq )
        f = _absoluteFreq->getPositiveValue(freq);
    else
        f = freq * _freqFactor->getPositiveValue(freq);

    double ret =
        _function(
                2. * M_PI * f *
                std::fmod(
                    ((double)_progress / (double)sampleRate),
                    1./f) +
                _freqModulation->getValue(freq)) *
            _amplitudeFactor->getPositiveValue(freq) +
        _amplitudeOffset->getValue(freq);

    _progress++;

    return ret;
}

double Sound::getPositiveInstantValue(double freq) {
    static size_t sampleRate = ExecutorInterface::getSampleRate();
    double f;

    if( _absoluteFreq )
        f = _absoluteFreq->getPositiveValue(freq);
    else
        f = freq * _freqFactor->getPositiveValue(freq);

    double ampOffset = _amplitudeOffset->getValue(freq);
    double ampFactor = _amplitudeFactor->getPositiveValue(freq);
    double ret =
        _function(
            2. * M_PI * f *
                std::fmod(
                    ((double)_progress / (double)sampleRate),
                    1./f) +
            _freqModulation->getValue(freq)) *
            ampFactor +
        ampOffset;

    _progress++;

    return ( ret + ampFactor - ampOffset ) / 2;
}

Sample::Sample(string fileName) {
    _fileName = fileName;
    _amplitudeFactor = new ConstModifier(1);
    _amplitudeOffset = new ConstModifier(0);
    _timeFactor = new ConstModifier(1);
    _panning = new ConstModifier(0.5);
}

Sample::Sample(const Sample& sample) {
    _progress = sample._progress;
    _isPlayed = sample._isPlayed;
    _startTick = sample._startTick;
    _fileName = sample._fileName;
    _amplitudeFactor = sample._amplitudeFactor->clone();
    _amplitudeOffset = sample._amplitudeOffset->clone();
    _timeFactor = sample._timeFactor->clone();
    _panning = sample._panning->clone();
}

Sample::~Sample() {
    if( _amplitudeFactor ) delete _amplitudeFactor;
    if( _timeFactor ) delete _timeFactor;
    if( _amplitudeOffset ) delete _amplitudeOffset;
    if( _panning ) delete _panning;
}

void Sample::play(tick_t currentTick, Buffer& bufferToLoad) {
    size_t bufferSize = bufferToLoad[Channel::right].size();

    signed long span = currentTick - _startTick;

    size_t cursor = 0;

    if( span <= 0 )
    {
        if( abs(span) >= (signed long)bufferSize )
            return;
        cursor = -span;
    }

    const AudioFile<float>* audioFile = Context::getInstance()->getAudioFile(_fileName);
    tick_t fileLength = audioFile->getNumSamplesPerChannel();

    double timeFactor = _timeFactor->getPositiveValue(0);

    for( ; cursor < bufferSize; cursor++ )
    {
        double valueLeft, valueRight;
        valueRight =
            (
            ( ( _progress - _progress/(double)timeFactor ) /
            ( (_progress + 1)/(double)timeFactor - _progress/(double)timeFactor ) ) *
            ( (double)audioFile->samples[Channel::right][(_progress+1)/(double)timeFactor] -
            (double)audioFile->samples[Channel::right][(_progress)/(double)timeFactor] ) +
            (double)audioFile->samples[Channel::right][(_progress)/(double)timeFactor] ) *
            _amplitudeFactor->getPositiveValue(0) + _amplitudeOffset->getValue(0);

        if( audioFile->isMono() )
        {
            valueLeft = valueRight;
        }
        else
        {
            valueLeft =
                (
                ( ( _progress - _progress/(double)timeFactor ) /
                ( (_progress + 1)/(double)timeFactor - _progress/(double)timeFactor ) ) *
                ( (double)audioFile->samples[Channel::left][(_progress+1)/(double)timeFactor] -
                (double)audioFile->samples[Channel::left][(_progress)/(double)timeFactor] ) +
                (double)audioFile->samples[Channel::left][(_progress)/(double)timeFactor] ) *
                _amplitudeFactor->getPositiveValue(0) + _amplitudeOffset->getValue(0);
        }

        if( _progress >= fileLength * timeFactor - 1 )
        {
            _isPlayed = true;
            return;
        }
        _progress++;

        bufferToLoad[Channel::right][cursor] = _panning->getPositiveValue(0) * valueRight;
        bufferToLoad[Channel::left][cursor] = (1 - _panning->getPositiveValue(0)) * valueLeft;
    }
}

void Sample::setAmplitudeFactor(double factor) {
    if( _amplitudeFactor ) delete _amplitudeFactor;
    _amplitudeFactor = new ConstModifier(factor);
}

void Sample::setAmplitudeFactor(Sound sound) {
    if( _amplitudeFactor ) delete _amplitudeFactor;
    _amplitudeFactor = new SoundModifier(sound);
}

void Sample::setAmplitudeOffset(double offset) {
    if( _amplitudeOffset ) delete _amplitudeOffset;
    _amplitudeOffset = new ConstModifier(offset);
}

void Sample::setAmplitudeOffset(Sound sound) {
    if( _amplitudeOffset ) delete _amplitudeOffset;
    _amplitudeOffset = new SoundModifier(sound);
}

void Sample::setTimeFactor(double offset) {
    if( _timeFactor ) delete _timeFactor;
    _timeFactor = new ConstModifier(offset);
}

void Sample::setTimeFactor(Sound sound) {
    if( _timeFactor) delete _timeFactor;
    _timeFactor = new SoundModifier(sound);
}

void Sample::setPanning(double pan) {
    if( _panning ) delete _panning;
    _panning = new ConstModifier(pan);
}

void Sample::setPanning(Sound sound) {
    if( _panning ) delete _panning;
    _panning = new SoundModifier(sound);
}

Sample* Sample::generate(double timeFactor,tick_t startTick) {
    Sample* ret = new Sample(*this);
    ret->_startTick = startTick;
    ret->setTimeFactor(timeFactor);
    return ret;
}

double Sample::getDurationInSeconds() {
    return Context::getInstance()->getAudioFile(_fileName)->getLengthInSeconds();
}

ConstModifier::ConstModifier(double value) {
    _value = value;
}

ConstModifier::~ConstModifier() {}

double ConstModifier::getValue(double) {
    return _value;
}

double ConstModifier::getPositiveValue(double) {
    if( _value >= 0 )
        return _value;
    return - _value;
}

bool ConstModifier::setAmplitudeOffset(Sound) {
    return false;
}

bool ConstModifier::setAmplitudeFactor(Sound) {
    return false;
}

Modifier* ConstModifier::clone() {
    return new ConstModifier(_value);
}

SoundModifier::SoundModifier(Sound sound) : _sound(sound) {}

SoundModifier::~SoundModifier() {}

double SoundModifier::getValue(double freq) {
    return _sound.getInstantValue(freq);
}

double SoundModifier::getPositiveValue(double freq) {
    return _sound.getPositiveInstantValue(freq);
}

bool SoundModifier::setAmplitudeOffset(Sound sound) {
    _sound.setAmplitudeOffset(sound);
    return true;
}

bool SoundModifier::setAmplitudeFactor(Sound sound) {
    _sound.setAmplitudeFactor(sound);
    return true;
}

Modifier* SoundModifier::clone() {
    return new SoundModifier(_sound);
}
