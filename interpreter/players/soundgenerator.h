#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include <string>
#include <list>
#include "executor/buffer.h"

namespace CS {

class SoundDataType;
class SamplePlayer;

class SoundGenerator
{
public:
    friend SamplePlayer;
    friend SoundDataType;
    SoundGenerator(double(*)(double));
    SoundGenerator(Sound);
    SoundGenerator(const SoundGenerator&);
    SoundGenerator* clone();
    void play(std::list<double>,double,tick_t,std::string);
    void loop(std::list<double>,double,double,tick_t,std::string);
    void stop(std::string);
    SoundGenerator operator*(double);
    SoundGenerator operator/(double);
    SoundGenerator operator*(SoundGenerator&);
    SoundGenerator operator+(SoundGenerator&);
    Sound getSound();
private:
    Sound _baseSound;
    std::list<Sound*> _generatedSounds;
};

}

#endif // SOUNDGENERATOR_H
