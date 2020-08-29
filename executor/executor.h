#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <list>
#include <stack>
#include "buffer.h"

using namespace std;

namespace CS {

/**
 * @brief This class manages the interface with jack library. It has methods to manage what sounds are played.
 */
class Executor
{
public:
    static Executor* getInstance();
    ~Executor();

    /**
     * @brief Add the sound passed as parameter into a queue, waiting to be added later into the reproduction list.
     * @param newSound Poiter to a Sound to be played
     */
    void addSound(Playable*) const;
    /**
     * @brief This method must be called first of all.
     * It start jack client and connect porst to the jack server and start a thread to handle queue and reproduction list sync
     */
    void init();

    unsigned int getSampleRate() const;
    char getSoundId();
private:
    Executor();
    static Executor* _instance;

    list<Playable*> _soundsList;
    stack<char> _availableSounds;
};

}
#endif // EXECUTOR_H
