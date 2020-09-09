#include "sampleplayer.h"
#include "executor/buffer.h"
#include "executor/executorinterface.h"
#include "interpteter/soundgenerator.h"

using namespace CS;

SamplePlayer::SamplePlayer(Sample sample) : _sample(sample) {}

SamplePlayer::SamplePlayer(const SamplePlayer& base) : _sample(base._sample) {}

double SamplePlayer::getDurationInSeconds() {
    return _sample.getDurationInSeconds();
}

void SamplePlayer::play(double timeFactor,tick_t startTick) {
    Sample* newSample = _sample.generate(timeFactor, startTick);
    _generatedSounds.push_back(newSample);
    ExecutorInterface::addSound(newSample);
}

SamplePlayer* SamplePlayer::clone() {
    return new SamplePlayer(*this);
}

void SamplePlayer::stop() {
    auto i = _generatedSounds.begin();

    while (i != _generatedSounds.end())
    {
        ExecutorInterface::removeSound(*i);
        _generatedSounds.erase(i);
    }
}

SamplePlayer SamplePlayer::operator*(double factor) {
    Sample copySound = _sample;
    copySound.setAmplitudeFactor(factor);
    SamplePlayer ret(copySound);
    return ret;
}

SamplePlayer SamplePlayer::operator/(double factor) {
    Sample copySound = _sample;
    copySound.setAmplitudeFactor(1/factor);
    SamplePlayer ret(copySound);
    return ret;
}

SamplePlayer SamplePlayer::operator*(SoundGenerator& sound) {
    Sample copySample = _sample;
    copySample.setAmplitudeFactor(sound._baseSound);
    SamplePlayer ret(copySample);
    return ret;
}

SamplePlayer SamplePlayer::operator+(SoundGenerator& sound) {
    Sample copySample = _sample;
    copySample.setAmplitudeOffset(sound._baseSound);
    SamplePlayer ret(copySample);
    return ret;
}
