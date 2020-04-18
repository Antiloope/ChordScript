#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <jack/jack.h>
#include "buffer.h"

using namespace std;
using namespace CS;

class AudioHandler
{
public:
    static AudioHandler* getInstance();
    ~AudioHandler();
    bool init();
    void outputBufferOverflow();
private:
    static AudioHandler* instance;
    AudioHandler();
    jack_client_t *_pJackClient;
    const char *_sJackClientName = "simple";
    const char *_sJackServerName = nullptr;
    vector<Sound*> _buffersList;
};

#endif // AUDIOHANDLER_H
