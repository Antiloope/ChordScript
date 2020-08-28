#include "buffer.h"
#include "executorinterface.h"
#include <math.h>

using namespace CS;

Buffer::Buffer() {
    _buffer = nullptr;
}

Buffer::Buffer(AudioBuffer audioBuffer) {
    _buffer = new AudioBuffer(audioBuffer);
}

Buffer::~Buffer() {
    if( _buffer ) delete _buffer;
}

vector<float>& Buffer::operator[](Channel a) {
    return (*_buffer)[a];
}

bool OutputBuffer::play(Sound* sound) {
    if( !sound ) return false;

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
    {
        return false;
    }
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
    _freqOffset = sound._freqOffset->clone();
    _absoluteFreq = sound._absoluteFreq ? sound._absoluteFreq->clone() :  nullptr;
    _panning = sound._panning->clone();
}

Sound::Sound(double(*function)(double)) {
    _function = function;
    _amplitudeFactor = new ConstModifier(1);
    _amplitudeOffset = new ConstModifier(0);
    _freqFactor = new ConstModifier(1);
    _freqOffset = new ConstModifier(0);
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
    _freqOffset = new ConstModifier(0);
    _panning = new ConstModifier(0.5);
}

Sound::~Sound() {
    if( _amplitudeFactor ) delete _amplitudeFactor;
    if( _freqFactor ) delete _freqFactor;
    if( _amplitudeOffset ) delete _amplitudeOffset;
    if( _freqOffset ) delete _freqOffset;
    if( _absoluteFreq ) delete _absoluteFreq;
}

bool Sound::isPlayed() const {
    return _isPlayed;
}

void Sound::setAmplitudeFactor(double factor) {
    if( _amplitudeFactor ) delete _amplitudeFactor;
    _amplitudeFactor = new ConstModifier(factor);
}

void Sound::setAmplitudeFactor(Sound sound) {
    if( _amplitudeFactor ) delete _amplitudeFactor;
    _amplitudeFactor = new SoundModifier(sound);
}

void Sound::setAmplitudeOffset(double offset) {
    if( _amplitudeOffset ) delete _amplitudeOffset;
    _amplitudeOffset = new ConstModifier(offset);
}

void Sound::setAmplitudeOffset(Sound sound) {
    if( _amplitudeOffset ) delete _amplitudeOffset;
    _amplitudeOffset = new SoundModifier(sound);
}

void Sound::setFreqFactor(double factor) {
    if( _freqFactor ) delete _freqFactor;
    _freqFactor = new ConstModifier(factor);
}

void Sound::setFreqFactor(Sound sound) {
    if( _freqFactor ) delete _freqFactor;
    _freqFactor = new SoundModifier(sound);
}

void Sound::setFreqOffset(double offset) {
    if( _freqOffset ) delete _freqOffset;
    _freqOffset = new ConstModifier(offset);
}

void Sound::setFreqOffset(Sound sound) {
    if( _freqOffset ) delete _freqOffset;
    _freqOffset = new SoundModifier(sound);
}

void Sound::setAbsoluteFreq(double freq) {
    if( _absoluteFreq ) delete _absoluteFreq;
    _absoluteFreq = new ConstModifier(freq);
}

void Sound::setAbsoluteFreq(Sound sound) {
    if( _absoluteFreq ) delete _absoluteFreq;
    _absoluteFreq = new SoundModifier(sound);
}

void Sound::setPanning(double pan) {
    if( _panning ) delete _panning;
    _panning = new ConstModifier(pan);
}

void Sound::setPanning(Sound sound) {
    if( _panning ) delete _panning;
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
    size_t bufferSize = bufferToLoad[Channel::right].size();

    signed long span = currentTick - _startTick;

    size_t cursor = 0;

    tick_t durationTicks = (tick_t)((double)_duration * (double)ExecutorInterface::getSampleRate());

    if( span <= 0 )
    {
        if( abs(span) >= (signed long)bufferSize )
        {
            return;
        }
        cursor = -span;
    }
    size_t sampleRate = ExecutorInterface::getSampleRate();
    for( ; cursor < bufferSize; cursor++ )
    {
        double freq;
        if( _absoluteFreq )
        {
            freq = _absoluteFreq->getPositiveValue(_freq);
        }
        else
        {
            freq = _freq * _freqFactor->getPositiveValue(_freq) + _freqOffset->getValue(_freq);
        }
        double value =
            _function(
                2. * M_PI * freq *
                ((double)_progress / (double)sampleRate)
                ) *
                _amplitudeFactor->getPositiveValue(_freq) + _amplitudeOffset->getValue(_freq);

        if( _progress >= durationTicks - 1000 )
        {
            double factor = ((double)durationTicks - (double)_progress) / (1000);
            value = value * factor;
        }
        if( _progress < 1000 )
        {
            double factor = (double)((double)_progress) / (1000);
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
    static double sampleRate = ExecutorInterface::getSampleRate();
    double f;
    if( _absoluteFreq )
    {
        f = _absoluteFreq->getPositiveValue(freq);
    }
    else
    {
        f = freq * _freqFactor->getPositiveValue(freq) + _freqOffset->getValue(freq);
    }
    double ret =
        _function(
            2. * M_PI * f *( (double)_progress / (double)sampleRate )
            ) *
            _amplitudeFactor->getPositiveValue(freq) + _amplitudeOffset->getValue(freq);
    _progress++;
    return ret;
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

Modifier* ConstModifier::clone() {
    return new ConstModifier(_value);
}

SoundModifier::SoundModifier(Sound sound) : _sound(sound) {}

SoundModifier::~SoundModifier() {}

double SoundModifier::getValue(double freq) {
    return _sound.getInstantValue(freq);
}

double SoundModifier::getPositiveValue(double freq) {
    return (_sound.getInstantValue(freq) + 1.) / 2 ;
}

Modifier* SoundModifier::clone() {
    return new SoundModifier(_sound);
}
