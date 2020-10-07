#include "sound.h"
#include "executor/modifiers/modifier.h"
#include "periodicsound.h"
#include "executor/executorinterface.h"
#include <math.h>
#include "executor/buffer/buffer.h"

using namespace CS;
using namespace std;

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
    if( _freqModulation )
        delete _freqModulation;

    _freqModulation = new SoundModifier(sound);
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

PeriodicSound* Sound::generate(double freq, double duration, double totalLoopDuration, tick_t startTick) {
    PeriodicSound* ret = new PeriodicSound(*this);
    ret->load(freq,duration,totalLoopDuration,startTick);
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

    size_t bufferSize = bufferToLoad[Channel::Right].size();

    signed long span = currentTick - _startTick;

    size_t cursor = 0;

    tick_t durationTicks = (tick_t)((double)_duration * (double)ExecutorInterface::getSampleRate());
    bool isPeriodic = false;
    if( _duration == 0 )
        isPeriodic = true;

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

        if( !isPeriodic && _progress >= durationTicks - 1000 )
        {
            double factor = ((double)durationTicks - (double)_progress) / (1000);
            value = value * factor;
        }
        if( _progress < 1000 )
        {
            double factor = ((double)_progress) / (1000.);
            value = value * factor;
        }
        if( !isPeriodic && _progress >= durationTicks )
        {
            if( abs(value) < 0.001 )
            {
                _isPlayed = true;
                return;
            }
        }
        _progress++;

        bufferToLoad[Channel::Right][cursor] = _panning->getPositiveValue(_freq) * value;
        bufferToLoad[Channel::Left][cursor] = (1 - _panning->getPositiveValue(_freq)) * value;
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
