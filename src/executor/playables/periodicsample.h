#ifndef PERIODICSAMPLE_H
#define PERIODICSAMPLE_H

#include "sample.h"

namespace CS {

class PeriodicSample : public Sample
{
public:
    friend Sample;
    PeriodicSample(const Sample&);
    PeriodicSample(const PeriodicSample&);
    PeriodicSample(std::string);
    virtual ~PeriodicSample();

    void play(tick_t, Buffer&) override;
protected:
    double _period;
};

}

#endif // PERIODICSAMPLE_H
