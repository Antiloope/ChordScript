#ifndef SOUND_H
#define SOUND_H

#include "playable.h"

namespace CS {

class Modifier;
class PeriodicSound;

class Sound : public Playable
{
public:
    Sound(const Sound&);
    Sound(double(*)(double));
    Sound(double(*)(double), double, double, tick_t);
    virtual ~Sound();

    virtual void play(tick_t, Buffer&) override;
    double getInstantValue(double);
    double getPositiveInstantValue(double);

    void setAmplitudeFactor(double);
    void setAmplitudeModulation(Sound);
    void setFreqFactor(double);
    void setFreqModulation(Sound);
    void setAbsoluteFreq(double);
    void clearAbsoluteFreq();
    void setPanning(double);
    void setPanning(Sound);
    void addSound(Sound);

    Sound* generate(double, double, tick_t);
    PeriodicSound* generate(double, double, double, tick_t);
protected:
    void load(double, double, tick_t);

    double (*_function)(double);
    double _freq = 0;
    double _duration = 0;

    Modifier* _panning;
    Modifier* _amplitudeFactor;
    Modifier* _amplitudeModulation;
    Modifier* _addedSound;
    Modifier* _freqFactor;
    Modifier* _freqModulation;
    Modifier* _absoluteFreq = nullptr;
};

}

#include "periodicsound.h"

#endif // SOUND_H
