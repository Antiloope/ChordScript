#ifndef SAMPLE_H
#define SAMPLE_H

#include "playable.h"
#include <string>

namespace CS {

class Sound;
class Modifier;
class PeriodicSample;

class Sample : public Playable
{
public:
    Sample(std::string);
    Sample(const Sample&);
    ~Sample();

    void play(tick_t, Buffer&) override;

    void setAmplitudeFactor(double);
    void setAmplitudeFactor(Sound);
    void setAmplitudeOffset(double);
    void setAmplitudeOffset(Sound);
    void setTimeFactor(double);
    void setTimeFactor(Sound);
    void setPanning(double);
    void setPanning(Sound);
    Sample* generate(double,tick_t);
    PeriodicSample* generate(double, tick_t, tick_t);
    double getDurationInSeconds();
protected:
    std::string _fileName;

    Modifier* _panning;
    Modifier* _amplitudeFactor;
    Modifier* _amplitudeOffset;
    Modifier* _timeFactor;
};

}

#include "periodicsample.h"

#endif // SAMPLE_H
