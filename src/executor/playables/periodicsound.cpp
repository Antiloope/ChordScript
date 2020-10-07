#include "periodicsound.h"
#include "executor/executorinterface.h"
#include "executor/buffer/buffer.h"
#include <math.h>
#include "executor/modifiers/modifier.h"

using namespace CS;
using namespace std;

PeriodicSound::PeriodicSound(const Sound& sound) : Sound(sound) {
    _period = 0;
}

PeriodicSound::PeriodicSound(const PeriodicSound& sound) : Sound(sound) {
    _period = sound._period;
}

PeriodicSound::PeriodicSound(double(*function)(double), double freq, double duration, double totalLoopDuration, tick_t startTick) :
    Sound(function,freq,duration,startTick) {
    _period = totalLoopDuration;
}

PeriodicSound::~PeriodicSound() {}

void PeriodicSound::play(tick_t currentTick,Buffer& bufferToLoad) {
    static double sampleRate = ExecutorInterface::getSampleRate();

    size_t bufferSize = bufferToLoad[Channel::Right].size();

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
                _startTick += _period * (double)ExecutorInterface::getSampleRate();
                _progress = 0;
                return;
            }
        }
        _progress++;

        bufferToLoad[Channel::Right][cursor] = _panning->getPositiveValue(_freq) * value;
        bufferToLoad[Channel::Left][cursor] = (1 - _panning->getPositiveValue(_freq)) * value;
    }
}

void PeriodicSound::load(double freq, double duration, double totalLoopDuration, tick_t startTick) {
    _freq = freq;
    _duration = duration;
    _startTick = startTick;
    _progress = 0;
    _isPlayed = false;
    _period = totalLoopDuration;
}
