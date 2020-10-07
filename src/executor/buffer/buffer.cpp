#include "buffer.h"

using namespace CS;
using namespace std;

Buffer::Buffer() {
    _buffer = nullptr;
}

Buffer::Buffer(AudioBuffer audioBuffer) {
    _buffer = new AudioBuffer(audioBuffer);
}

Buffer::~Buffer() {
    if( _buffer )
        delete _buffer;
}

AudioBuffer* Buffer::getBuffer() {
    return _buffer;
}

vector<float>& Buffer::operator[](Channel a) {
    return (*_buffer)[a];
}
