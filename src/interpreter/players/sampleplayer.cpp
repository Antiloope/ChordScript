#include "sampleplayer.h"
#include "executor/executorinterface.h"
#include "interpreter/players/soundgenerator.h"

using namespace CS;

SamplePlayer::SamplePlayer(Sample sample) : _sample(sample) {}

SamplePlayer::SamplePlayer(const SamplePlayer& base) : _sample(base._sample) {}

double SamplePlayer::getDurationInSeconds() {
    return _sample.getDurationInSeconds();
}

void SamplePlayer::play(double timeFactor, tick_t startTick, string variableName) {
    Sample* newSample = _sample.generate(timeFactor, startTick);
    _generatedSounds.push_back(newSample);
    ExecutorInterface::addSound(newSample,variableName);
}

void SamplePlayer::loop(double timeFactor,tick_t startTick,string variableName) {
    PeriodicSample* newSample = _sample.generate(
        timeFactor,
        TimeHandler::getInstance()->segToTicks(timeFactor * getDurationInSeconds()),
        startTick);
    _generatedSounds.push_back(newSample);
    ExecutorInterface::addSound(newSample,variableName);
}

void SamplePlayer::loop(double timeFactor,double period,tick_t startTick,string variableName) {
    PeriodicSample* newSample = _sample.generate(
        timeFactor,
        TimeHandler::getInstance()->segToTicks(period),
        startTick);
    _generatedSounds.push_back(newSample);
    ExecutorInterface::addSound(newSample,variableName);
}

SamplePlayer* SamplePlayer::clone() {
    return new SamplePlayer(*this);
}

void SamplePlayer::stop(string variableName) {
    ExecutorInterface::removeSound(variableName);
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
