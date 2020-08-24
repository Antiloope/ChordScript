#include "soundgenerator.h"
#include "context.h"

using namespace CS;

SoundGenerator::SoundGenerator(double(*function)(double)) : _baseSound(function) {}

SoundGenerator::SoundGenerator(Sound sound) : _baseSound(sound) {}

SoundGenerator::SoundGenerator(const SoundGenerator& base) : _baseSound(base._baseSound) {}

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
    copySound.setAmplitudeFactor(sound._baseSound);
    SoundGenerator ret(copySound);
    return ret;
}

SoundGenerator SoundGenerator::operator+(SoundGenerator& sound) {
    Sound copySound = _baseSound;
    copySound.setAmplitudeOffset(sound._baseSound);
    SoundGenerator ret(copySound);
    return ret;
}

SoundGenerator SoundGenerator::operator-(SoundGenerator& sound) {
    Sound copySound = _baseSound;
    copySound.setAmplitudeOffset(sound._baseSound);
    SoundGenerator ret(copySound);
    return ret;
}

void SoundGenerator::play(list<double> freqList,double duration,tick_t startTick) {
    if( freqList.empty() )
    {
        Sound* newSound = _baseSound.generate(0,duration,startTick);
        _generatedSounds.push_back(newSound);

        ExecutorInterface::addSound(newSound);
    }
    else
    {
        for( double freq : freqList )
        {
            Sound* newSound = _baseSound.generate(freq,duration,startTick);
            _generatedSounds.push_back(newSound);

            ExecutorInterface::addSound(newSound);
        }
    }
}

void SoundGenerator::stop() {
    auto i = _generatedSounds.begin();

    while (i != _generatedSounds.end())
    {
        delete (*i);
        _generatedSounds.erase(i);
    }
}
