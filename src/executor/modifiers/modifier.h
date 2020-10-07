#ifndef MODIFIER_H
#define MODIFIER_H

namespace CS {

class Sound;

class Modifier
{
public:
    Modifier() {};
    virtual ~Modifier() {};
    virtual double getValue(double) = 0;
    virtual double getPositiveValue(double) = 0;
    virtual bool setAmplitudeOffset(Sound) = 0;
    virtual bool setAmplitudeFactor(Sound) = 0;
    virtual Modifier* clone() = 0;
private:
};

}

#include "constmodifier.h"
#include "soundmodifier.h"

#endif // MODIFIER_H
