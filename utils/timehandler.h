#ifndef TIMEHANDLER_H
#define TIMEHANDLER_H

namespace CS {

typedef unsigned long long tick_t;

class TimeHandler
{
public:
    ~TimeHandler();
    static TimeHandler *getInstance();

    void increaseTick(unsigned long ticks);
    bool configure(unsigned long sampleRate, unsigned long bufferSize);
    tick_t getCurrentTick() const;
    tick_t msToTicks(unsigned long time_ms) const;
    tick_t segToTicks(double time_seg) const;
    unsigned long ticksToMs(tick_t) const;
private:
    TimeHandler();
    static TimeHandler *instance;

    tick_t _tick;
    unsigned long _sampleRate;
    unsigned long _bufferSize;
    double _tickTime;
};
}

#endif // TIMEHANDLER_H
