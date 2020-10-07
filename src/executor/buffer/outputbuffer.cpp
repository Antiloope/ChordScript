#include "outputbuffer.h"
#include "executor/playables/playable.h"

using namespace CS;
using namespace std;

bool OutputBuffer::play(Playable* sound) {
    if( !sound )
        return false;

    size_t bufferSize = (*_buffer)[Channel::Right].size();

    Buffer newBuffer(AudioBuffer(2));
    newBuffer[Channel::Right].resize(bufferSize);
    newBuffer[Channel::Left].resize(bufferSize);

    sound->play(TimeHandler::getInstance()->getCurrentTick(),newBuffer);

    BufferChannel* rightChannel = &(*_buffer)[Channel::Right];
    BufferChannel* leftChannel = &(*_buffer)[Channel::Left];

    for( size_t i = 0; i < bufferSize; i++ )
    {
        (*rightChannel)[i] = (*rightChannel)[i] + newBuffer[Channel::Right][i];
        (*leftChannel)[i] = (*leftChannel)[i] + newBuffer[Channel::Left][i];
    }

    if( sound->isPlayed() )
        return false;

    return true;
}

OutputBuffer::OutputBuffer() {
    _buffer = new AudioBuffer();
    _buffer->resize(2);
}

void OutputBuffer::setSize(size_t size) {
    (*_buffer)[0].resize(size);
    (*_buffer)[1].resize(size);
}

OutputBuffer::~OutputBuffer() {}

void OutputBuffer::reset() {
    for(size_t i = 0 ; i < (*_buffer)[Channel::Right].size(); i++)
    {
        (*_buffer)[Channel::Right][i] = 0.f;
        (*_buffer)[Channel::Left][i] = 0.f;
    }
}
