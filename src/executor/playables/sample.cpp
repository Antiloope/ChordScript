#include "sample.h"
#include "executor/modifiers/modifier.h"
#include "interpreter/context.h"
#include "executor/buffer/buffer.h"

using namespace CS;
using namespace std;

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

    for( ; cursor < bufferSize; cursor++ )
    {
        double valueLeft, valueRight;
        valueRight =
            (
                ( ( _progress - _progress/(double)timeFactor ) /
                 ( (_progress + 1)/(double)timeFactor - _progress/(double)timeFactor ) ) *
                    ( (double)audioFile->samples[Channel::Right][(_progress+1)/(double)timeFactor] -
                     (double)audioFile->samples[Channel::Right][(_progress)/(double)timeFactor] ) +
                (double)audioFile->samples[Channel::Right][(_progress)/(double)timeFactor] ) *
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
                        ( (double)audioFile->samples[Channel::Left][(_progress+1)/(double)timeFactor] -
                         (double)audioFile->samples[Channel::Left][(_progress)/(double)timeFactor] ) +
                    (double)audioFile->samples[Channel::Left][(_progress)/(double)timeFactor] ) *
                    _amplitudeFactor->getPositiveValue(0) + _amplitudeOffset->getValue(0);
        }

        if( _progress >= fileLength * timeFactor - 1 )
        {
            _isPlayed = true;
            return;
        }
        _progress++;

        bufferToLoad[Channel::Right][cursor] = _panning->getPositiveValue(0) * valueRight;
        bufferToLoad[Channel::Left][cursor] = (1 - _panning->getPositiveValue(0)) * valueLeft;
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
