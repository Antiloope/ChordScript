#ifndef BUFFER_H
#define BUFFER_H
#include <jack/jack.h>
#include "AudioFile.h"
#include "utils/timehandler.h"

namespace CS {

constexpr unsigned short BUFFER_RIGHT = 0;
constexpr unsigned short BUFFER_LEFT = 1;

constexpr unsigned short NOW = 0;

typedef AudioFile<float>::AudioBuffer AudioBuffer;

class Buffer
{
public:
    Buffer();
    Buffer(AudioBuffer);
    virtual ~Buffer();

    std::vector<float>& operator[](int a);
protected:
    AudioBuffer *_buffer;
};

class Sound : public Buffer
{
public:
    Sound(unsigned long time_ms);
    Sound(AudioBuffer,unsigned long time_ms);
    ~Sound();

    friend class OutputBuffer;
    virtual bool play(size_t);
    virtual tick_t getStartTick();

protected:
    unsigned long _progress;
    tick_t _startTick;
    bool _isPlayed;
private:
};

class PeriodicSound : public Sound
{
public:
    PeriodicSound();
    ~PeriodicSound();

    bool play(size_t);
    tick_t getStartTick();
private:

};

class OutputBuffer : public Buffer
{
public:
    OutputBuffer();
    OutputBuffer(AudioBuffer);
    ~OutputBuffer();

    bool play(Sound*);
    void setSize(unsigned long);
    void reset();
};

}

#endif // BUFFER_H
