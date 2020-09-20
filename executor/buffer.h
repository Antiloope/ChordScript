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
    AudioBuffer* getBuffer();
    BufferChannel& operator[](Channel a);
protected:
    AudioBuffer *_buffer;
};

class Playable
{
public:
    Playable() {};
    virtual ~Playable() {};
    virtual void play(tick_t, Buffer&) = 0;
    virtual bool isPlayed() const;
protected:
    tick_t _progress = 0;
    tick_t _startTick = 0;
    bool _isPlayed = false;
};

class Sound : public Playable
{
public:
    Sound(const Sound&);
    Sound(double(*)(double));
    Sound(double(*)(double), double, double, tick_t);
    virtual ~Sound();

    void play(tick_t, Buffer&) override;
    inline double getInstantValue(double);
    inline double getPositiveInstantValue(double);

    void setAmplitudeFactor(double);
    void setAmplitudeFactor(Sound);
    void setAmplitudeOffset(double);
    void setAmplitudeOffset(Sound);
    void setFreqFactor(double);
    void setFreqModulation(Sound);
    void setAbsoluteFreq(double);
    void clearAbsoluteFreq();
    void setPanning(double);
    void setPanning(Sound);

    Sound* generate(double, double, tick_t);
protected:
    void load(double, double, tick_t);

    double (*_function)(double);
    double _freq = 0;
    double _duration = 0;

    Modifier* _panning;
    Modifier* _amplitudeFactor;
    Modifier* _amplitudeOffset;
    Modifier* _freqFactor;
    Modifier* _freqModulation;
    Modifier* _absoluteFreq = nullptr;
private:
};

class Sample : public Playable
{
public:
    Sample(string);
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
    double getDurationInSeconds();
private:
    string _fileName;

    Modifier* _panning;
    Modifier* _amplitudeFactor;
    Modifier* _amplitudeOffset;
    Modifier* _timeFactor;
};

class OutputBuffer : public Buffer
{
public:
    OutputBuffer();
    OutputBuffer(AudioBuffer);
    ~OutputBuffer();

    bool play(Playable*);
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
    virtual bool setAmplitudeOffset(Sound) = 0;
    virtual bool setAmplitudeFactor(Sound) = 0;
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
    bool setAmplitudeOffset(Sound) override;
    bool setAmplitudeFactor(Sound) override;
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
    bool setAmplitudeOffset(Sound) override;
    bool setAmplitudeFactor(Sound) override;
    Modifier * clone() override;
private:
    Sound _sound;
};


}

#endif // BUFFER_H
