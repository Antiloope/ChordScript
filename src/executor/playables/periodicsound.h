#ifndef PERIODICSOUND_H
#define PERIODICSOUND_H

#include "sound.h"

namespace CS {

class PeriodicSound : public Sound
{
public:
    friend Sound;
    PeriodicSound(const Sound&);
    PeriodicSound(const PeriodicSound&);
    PeriodicSound(double(*)(double), double, double, double, tick_t);
    virtual ~PeriodicSound();

    void play(tick_t, Buffer&) override;
protected:
    void load(double, double, double, tick_t);

    double _period;
private:
};

}

#endif // PERIODICSOUND_H
