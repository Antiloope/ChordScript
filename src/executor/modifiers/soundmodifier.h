#ifndef SOUNDMODIFIER_H
#define SOUNDMODIFIER_H

#include "modifier.h"
#include "executor/playables/sound.h"

namespace CS {

class SoundModifier : public Modifier
{
public:
    SoundModifier(Sound);
    ~SoundModifier();
    double getValue(double) override;
    double getPositiveValue(double) override;
    bool setAmplitudeModulation(Sound) override;
    Modifier * clone() override;
private:
    Sound _sound;
};

}

#endif // SOUNDMODIFIER_H
