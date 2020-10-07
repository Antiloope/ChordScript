#ifndef SAMPLEPLAYER_H
#define SAMPLEPLAYER_H

#include <string>
#include <list>
#include "executor/playables/sample.h"

using namespace std;

namespace CS {

class SoundGenerator;
class SampleDataType;

class SamplePlayer
{
public:
    friend SampleDataType;
    SamplePlayer(Sample);
    SamplePlayer(const SamplePlayer&);

    double getDurationInSeconds();
    void play(double,tick_t,std::string);
    void stop(std::string);
    SamplePlayer* clone();
    SamplePlayer operator*(double);
    SamplePlayer operator/(double);
    SamplePlayer operator*(SoundGenerator&);
    SamplePlayer operator+(SoundGenerator&);
private:
    Sample _sample;
    list<Sample*> _generatedSounds;
};

}

#endif // SAMPLEPLAYER_H
