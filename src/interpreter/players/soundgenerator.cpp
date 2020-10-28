#include "soundgenerator.h"
#include "interpreter/context.h"
#include "executor/executorinterface.h"

using namespace CS;
using namespace std;

SoundGenerator::SoundGenerator(double(*function)(double)) : _baseSound(function) {}

SoundGenerator::SoundGenerator(Sound sound) : _baseSound(sound) {}

SoundGenerator::SoundGenerator(const SoundGenerator& base) : _baseSound(base._baseSound),_generatedSounds(base._generatedSounds) {}

SoundGenerator* SoundGenerator::clone() {
    return new SoundGenerator(*this);
}

SoundGenerator SoundGenerator::operator*(double factor) {
    Sound copySound = _baseSound;
    copySound.setAmplitudeFactor(factor);
    SoundGenerator ret(copySound);
    return ret;
}

SoundGenerator SoundGenerator::operator/(double factor) {
    Sound copySound = _baseSound;
    copySound.setAmplitudeFactor(1/factor);
    SoundGenerator ret(copySound);
    return ret;
}

SoundGenerator SoundGenerator::operator*(SoundGenerator& sound) {
    Sound copySound = _baseSound;
    copySound.setAmplitudeModulation(sound._baseSound);
    SoundGenerator ret(copySound);
    return ret;
}

SoundGenerator SoundGenerator::operator+(SoundGenerator& sound) {
    Sound copySound = _baseSound;
    copySound.addSound(sound._baseSound);
    SoundGenerator ret(copySound);
    return ret;
}

void SoundGenerator::play(list<double> freqList,double duration,tick_t startTick,string variableName) {
    if( freqList.empty() )
    {
        Sound* newSound = _baseSound.generate(0,duration,startTick);

        ExecutorInterface::addSound(newSound,variableName);
    }
    else
    {
        for( double freq : freqList )
        {
            Sound* newSound = _baseSound.generate(freq,duration,startTick);

            ExecutorInterface::addSound(newSound,variableName);
        }
    }
}

void SoundGenerator::loop(list<double> freqList,double duration,double totalLoopDuration,tick_t startTick,string variableName) {
    if( freqList.empty() )
    {
        PeriodicSound* newSound = _baseSound.generate(0,duration,totalLoopDuration,startTick);

        ExecutorInterface::addSound(newSound,variableName);
    }
    else
    {
        for( double freq : freqList )
        {
            PeriodicSound* newSound = _baseSound.generate(freq,duration,totalLoopDuration,startTick);

            ExecutorInterface::addSound(newSound,variableName);
        }
    }
}


void SoundGenerator::stop(string variableName) {
    ExecutorInterface::removeSound(variableName);
}

Sound SoundGenerator::getSound() {
    return _baseSound;
}
