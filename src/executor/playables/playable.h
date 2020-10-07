#ifndef PLAYABLE_H
#define PLAYABLE_H

#include "utils/timehandler.h"

namespace CS {

class Buffer;

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

}

#include "sound.h"
#include "sample.h"

#endif // PLAYABLE_H
