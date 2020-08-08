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
    Sound(unsigned long time_ms);
    Sound(AudioBuffer,unsigned long time_ms);
    virtual ~Sound();

    virtual bool play(size_t);
    virtual tick_t getStartTick() const;
    tick_t getRemainingTicks();
    tick_t getAbsoluteProgress() const;
    float getInstantValue(Channel,tick_t);

    BufferChannel& operator[](Channel);
protected:
    tick_t _progress;
    tick_t _startTick;
    bool _isPlayed;
    Buffer _buffer;
private:
};

class PeriodicSound : public Sound
{
public:
    PeriodicSound();
    ~PeriodicSound();

    bool play(size_t);
    tick_t getStartTick() const;
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

}

#endif // BUFFER_H
