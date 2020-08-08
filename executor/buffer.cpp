#include "buffer.h"

using namespace CS;

Buffer::Buffer() {
    _buffer = nullptr;
}

Buffer::Buffer(AudioBuffer audioBuffer) {
    _buffer = new AudioBuffer(audioBuffer);
}

Buffer::~Buffer() {
    if( _buffer ) delete _buffer;
}

vector<float>& Buffer::operator[](Channel a) {
    return (*_buffer)[a];
}

bool OutputBuffer::play(Sound* sound) {
    tick_t currentTick = TimeHandler::getInstance()->getCurrentTick();
    tick_t tickPlusBuffer = currentTick + (*_buffer)[Channel::right].size();
    tick_t sizeToPlay = sound->getRemainingTicks();

    sizeToPlay = (sound->getStartTick() + sizeToPlay < (*_buffer)[Channel::right].size())
                     ? sound->getStartTick() + sizeToPlay
                     : (*_buffer)[Channel::right].size();

    while ( sound->getAbsoluteProgress() < tickPlusBuffer)
    {
        BufferChannel* rightChannel = &(*_buffer)[Channel::right];
        BufferChannel* leftChannel = &(*_buffer)[Channel::left];
        for (
            unsigned long i = sound->getStartTick() ;
            i < sizeToPlay;
            i++ )
        {
            (*rightChannel)[i] = (*rightChannel)[i] + sound->getInstantValue(Channel::right,i);
            (*leftChannel)[i] = (*leftChannel)[i] + sound->getInstantValue(Channel::left,i);
        }
        if ( !sound->play(sizeToPlay) )
        {
            return false;
        }
    }

    return true;
}

OutputBuffer::OutputBuffer() {
    _buffer = new AudioBuffer();
    _buffer->resize(2);
}

void OutputBuffer::setSize(unsigned long size) {
    (*_buffer)[0].resize(size);
    (*_buffer)[1].resize(size);
}

OutputBuffer::~OutputBuffer() {
    delete _buffer;
}

void OutputBuffer::reset() {
    for(size_t i = 0 ; i < (*_buffer)[Channel::right].size(); i++)
    {
        (*_buffer)[Channel::right][i] = 0.f;
        (*_buffer)[Channel::left][i] = 0.f;
    }
}

Sound::Sound(unsigned long time_ms) {
    _progress = 0;
    _isPlayed = false;
    _startTick = TimeHandler::getInstance()->getCurrentTick() + TimeHandler::getInstance()->msToTicks(time_ms);
}

Sound::Sound(AudioBuffer buffer,unsigned long time_ms) : _buffer(buffer) {
    _progress = 0;
    _isPlayed = false;
    _startTick = TimeHandler::getInstance()->getCurrentTick() + TimeHandler::getInstance()->msToTicks(time_ms);
}

Sound::~Sound() {

}

bool Sound::play(size_t playedTicks) {
    _progress += playedTicks;
    if ( _progress < _buffer[Channel::right].size())
    {
        return true;
    }

    _isPlayed = true;
    return false;
}

tick_t Sound::getStartTick() const {
    if (_progress != 0)
        return 0;
    return _startTick;
}

tick_t Sound::getRemainingTicks() {
    return _buffer[Channel::right].size() - _progress;
}

tick_t Sound::getAbsoluteProgress() const {
    return _progress + _startTick;
}

float Sound::getInstantValue(Channel channel, tick_t i) {
    return _buffer[channel][_progress + i];
}

PeriodicSound::PeriodicSound() : Sound(NOW) {

}

PeriodicSound::~PeriodicSound() {

}

bool PeriodicSound::play(size_t) {

}

tick_t PeriodicSound::getStartTick() const {

}
