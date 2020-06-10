#ifndef AUDIOSERVERADAPTER_H
#define AUDIOSERVERADAPTER_H

#include <list>
#include "buffer.h"

using namespace std;

namespace CS {

class AudioServerAdapter
{
public:
    static AudioServerAdapter* getInstance();
    ~AudioServerAdapter();
    // TODO: Configure parameters to init.
    // Using other methods? As paramters to init method?
    void init();
private:
    AudioServerAdapter();
    static AudioServerAdapter* _instance;

    list<Sound*> _soundsList;
};

}

#endif // AUDIOSERVERADAPTER_H
