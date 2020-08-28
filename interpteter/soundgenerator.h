#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include <string>
#include <list>
#include "executor/buffer.h"

using namespace std;

namespace CS {

class SoundDataType;

class SoundGenerator
{
public:
    friend SoundDataType;
    SoundGenerator(double(*)(double));
    SoundGenerator(Sound);
    SoundGenerator(const SoundGenerator&);
    SoundGenerator* clone();
    void play(list<double>,double,tick_t);
    void stop();
    SoundGenerator operator*(double);
    SoundGenerator operator/(double);
    SoundGenerator operator*(SoundGenerator&);
    SoundGenerator operator+(SoundGenerator&);
    Sound getSound();
private:
    Sound _baseSound;
    list<Sound*> _generatedSounds;
};

}

#endif // SOUNDGENERATOR_H
