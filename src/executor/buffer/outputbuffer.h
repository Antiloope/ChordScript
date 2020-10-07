#ifndef OUTPUTBUFFER_H
#define OUTPUTBUFFER_H

#include "buffer.h"

namespace CS {

class Playable;

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

}

#endif // OUTPUTBUFFER_H
