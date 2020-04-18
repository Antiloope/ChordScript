#include "timehandler.h"

using namespace CS;

TimeHandler* TimeHandler::instance = new TimeHandler();

TimeHandler::TimeHandler()
{
    _tick = 0;
}

TimeHandler::~TimeHandler()
{

}

TimeHandler* TimeHandler::getInstance(){
    if ( !instance ) instance = new TimeHandler();
    return instance;
}

void TimeHandler::increaseTick(unsigned long ticks)
{
    _tick += ticks;
}

bool TimeHandler::configure(unsigned long sampleRate, unsigned long bufferSize)
{
    if ( 0 == sampleRate || 0 == bufferSize)
        return false;

    _bufferSize = bufferSize;
    _sampleRate = sampleRate;

    _tickTime = (double)(1 / (double)sampleRate) * (double)bufferSize;
    return true;
}

tick_t TimeHandler::getCurrentTick() const{
    return _tick;
}

tick_t TimeHandler::msToTicks(unsigned long time_ms)
{
    return (tick_t)((double)time_ms / _tickTime);
}

unsigned long TimeHandler::ticksToMs(tick_t ticks)
{
    return ticks * _tickTime;
}
