#include "periodicsample.h"
#include "executor/buffer/buffer.h"
#include "interpreter/context.h"
#include "executor/modifiers/modifier.h"

using namespace CS;
using namespace std;

PeriodicSample::PeriodicSample(const Sample& sample) : Sample(sample) {
    _period = 0;
}

PeriodicSample::PeriodicSample(const PeriodicSample& sample) : Sample(sample) {
    _period = sample._period;
}

PeriodicSample::PeriodicSample(string fileName) : Sample(fileName) {
    _period = 0;
}

PeriodicSample::~PeriodicSample() {}

void PeriodicSample::play(tick_t currentTick, Buffer& bufferToLoad) {
    size_t bufferSize = bufferToLoad[Channel::Right].size();

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

    tick_t newSampleLength = (fileLength - 1) / timeFactor;

    for( ; cursor < bufferSize; cursor++ )
    {
        double progress = _progress * timeFactor;
        double valueLeft, valueRight;
        valueRight =
            audioFile->samples[Channel::Right][(unsigned)progress] +
            (
                audioFile->samples[Channel::Right][((unsigned)progress)+1] -
                audioFile->samples[Channel::Right][(unsigned)progress]
            ) *
            timeFactor *
            _amplitudeFactor->getPositiveValue(0) + _amplitudeOffset->getValue(0);

        if( audioFile->isMono() )
            valueLeft = valueRight;
        else
            valueLeft =
                audioFile->samples[Channel::Left][(unsigned)progress] +
                (
                    audioFile->samples[Channel::Left][((unsigned)progress)+1] -
                    audioFile->samples[Channel::Left][(unsigned)progress]
                ) *
                timeFactor *
                _amplitudeFactor->getPositiveValue(0) + _amplitudeOffset->getValue(0);

        if( _progress >= newSampleLength )
        {
            _startTick += _period;
            _progress = 0;
            return;
        }
        _progress++;

        bufferToLoad[Channel::Right][cursor] = _panning->getPositiveValue(0) * valueRight;
        bufferToLoad[Channel::Left][cursor] = (1 - _panning->getPositiveValue(0)) * valueLeft;
    }
}
