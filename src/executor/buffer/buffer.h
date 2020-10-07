#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include "executor/AudioFile.h"

namespace CS {

enum Channel {
    Right = 0,
    Left = 1
};

typedef AudioFile<float>::AudioBuffer AudioBuffer;
typedef std::vector<float> BufferChannel;

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

}

#include "outputbuffer.h"

#endif // BUFFER_H
