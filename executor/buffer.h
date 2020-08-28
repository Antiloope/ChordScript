#ifndef BUFFER_H
#define BUFFER_H

#include "AudioFile.h"
#include "utils/timehandler.h"

using namespace std;

namespace CS {

enum Channel {
    right = 0,
    left = 1
};

constexpr unsigned short NOW = 0;

typedef AudioFile<float>::AudioBuffer AudioBuffer;
typedef vector<float> BufferChannel;

class Modifier;

class Buffer
{
public:
    Buffer();
    Buffer(AudioBuffer);
    virtual ~Buffer();

    BufferChannel& operator[](Channel a);
protected:
    AudioBuffer *_buffer;
};

class Sound
{
public:
    Sound(const Sound&);
    Sound(double(*)(double));
    Sound(double(*)(double), double, double, tick_t);
    virtual ~Sound();

    virtual void play(tick_t, Buffer&);
    inline double getInstantValue(double);
    bool isPlayed() const;

    void setAmplitudeFactor(double);
    void setAmplitudeFactor(Sound);
    void setAmplitudeOffset(double);
    void setAmplitudeOffset(Sound);
    void setFreqFactor(double);
    void setFreqFactor(Sound);
    void setFreqOffset(double);
    void setFreqOffset(Sound);
    void setAbsoluteFreq(double);
    void setAbsoluteFreq(Sound);
    void setPanning(double);
    void setPanning(Sound);

    Sound* generate(double, double, tick_t);
protected:
    void load(double, double, tick_t);

    tick_t _progress = 0;
    tick_t _startTick = 0;
    bool _isPlayed = false;
    double (*_function)(double);
    double _freq = 0;
    double _duration = 0;

    Modifier* _panning;
    Modifier* _amplitudeFactor;
    Modifier* _amplitudeOffset;
    Modifier* _freqFactor;
    Modifier* _freqOffset;
    Modifier* _absoluteFreq = nullptr;
private:
};

class OutputBuffer : public Buffer
{
public:
    OutputBuffer();
    OutputBuffer(AudioBuffer);
    ~OutputBuffer();

    bool play(Sound*);
    void setSize(size_t);
    void reset();
};

class Modifier
{
public:
    Modifier() {};
    virtual ~Modifier() {};
    virtual double getValue(double) = 0;
    virtual double getPositiveValue(double) = 0;
    virtual Modifier* clone() = 0;
private:
};

class ConstModifier : public Modifier
{
public:
    ConstModifier(double);
    ~ConstModifier();
    double getValue(double) override;
    double getPositiveValue(double) override;
    Modifier * clone() override;
private:
    double _value;
};

class SoundModifier : public Modifier
{
public:
    SoundModifier(Sound);
    ~SoundModifier();
    double getValue(double) override;
    double getPositiveValue(double) override;
    Modifier * clone() override;
private:
    Sound _sound;
};


}

#endif // BUFFER_H
