#include "buffer.h"
#include <cstring>

using namespace CS;

Buffer::Buffer()
{
    _buffer = nullptr;
}

Buffer::Buffer(AudioBuffer audioBuffer)
{
    _buffer = new AudioBuffer(audioBuffer);
}

Buffer::~Buffer()
{
    delete _buffer;
}

std::vector<float>& Buffer::operator[](int a)
{
    return (*_buffer)[a];
}

bool OutputBuffer::play(Sound* sound)
{
    tick_t currentTick = TimeHandler::getInstance()->getCurrentTick();
    tick_t tickPlusBuffer = currentTick + (*_buffer)[BUFFER_RIGHT].size();
    tick_t sizeToPlay = (*sound)[BUFFER_RIGHT].size() - sound->_progress;

    sizeToPlay = (sound->getStartTick() + sizeToPlay < (*_buffer)[BUFFER_RIGHT].size()) ? sound->getStartTick() + sizeToPlay : (*_buffer)[BUFFER_RIGHT].size();

    while ( sound->_progress + sound->_startTick < tickPlusBuffer)
    {
        for (
            unsigned long i = sound->getStartTick() ;
            i < sizeToPlay;
            i++
            )
        {
            (*_buffer)[BUFFER_RIGHT][i] = (*_buffer)[BUFFER_RIGHT][i] + (*sound)[BUFFER_RIGHT][sound->_progress + i];
            (*_buffer)[BUFFER_LEFT][i] = (*_buffer)[BUFFER_LEFT][i] + (*sound)[BUFFER_LEFT][sound->_progress + i];
        }
        if (!sound->play(sizeToPlay))
        {
            return false;
        }
    }

    return true;
}

OutputBuffer::OutputBuffer()
{
    _buffer = new AudioBuffer();
    _buffer->resize(2);
}

void OutputBuffer::setSize(unsigned long size)
{
    (*_buffer)[0].resize(size);
    (*_buffer)[1].resize(size);
}

OutputBuffer::~OutputBuffer()
{
    delete _buffer;
}

void OutputBuffer::reset()
{
    for(size_t i = 0 ; i < (*_buffer)[BUFFER_RIGHT].size(); i++)
    {
        (*_buffer)[BUFFER_RIGHT][i] = 0.f;
        (*_buffer)[BUFFER_LEFT][i] = 0.f;
    }
}

Sound::Sound(unsigned long time_ms) : Buffer()
{
    _progress = 0;
    _isPlayed = false;
    _startTick = TimeHandler::getInstance()->getCurrentTick() + TimeHandler::getInstance()->msToTicks(time_ms);
}

Sound::Sound(AudioBuffer buffer,unsigned long time_ms) : Buffer(buffer)
{
    _progress = 0;
    _isPlayed = false;
    _startTick = TimeHandler::getInstance()->getCurrentTick() + TimeHandler::getInstance()->msToTicks(time_ms);
}

Sound::~Sound()
{

}

bool Sound::play(size_t playedTicks)
{
    _progress += playedTicks;
    if ( _progress < (*_buffer)[BUFFER_RIGHT].size())
    {
        return true;
    }

    _isPlayed = true;
    return false;
}

tick_t Sound::getStartTick()
{
    if (_progress != 0)
        return 0;
    return _startTick;
}

PeriodicSound::PeriodicSound() : Sound(NOW)
{

}

PeriodicSound::~PeriodicSound()
{

}

bool PeriodicSound::play(size_t)
{

}

tick_t PeriodicSound::getStartTick()
{

}
