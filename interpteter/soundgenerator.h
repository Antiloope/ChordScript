#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include <string>
#include <list>
#include "executor/buffer.h"

using namespace std;

namespace CS {

class SoundGenerator
{
public:
    SoundGenerator(string);
    SoundGenerator(const SoundGenerator&);
    SoundGenerator* clone();
    void play(list<double>,double,tick_t);
    void stop();
private:
    string _functionName;
    list<Sound*> _generatedSounds;
};

}

#endif // SOUNDGENERATOR_H
