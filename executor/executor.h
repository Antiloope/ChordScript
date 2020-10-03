#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <list>
#include <stack>
#include "buffer.h"
#include <functional>

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

    void removeSound(Playable*) const;

    void removeAllSounds() const;

    unsigned int getSampleRate() const;
    char getSoundId();

    void closeAll();

    void startRecording();
    void stopRecording();

    bool isServerRunning();
    void startServer();
    void restartServer();
    void killServer();

    int addObserver(function<void(const AudioBuffer&)>);
    void removeObserver(int);
private:
    static void loadBuffer(list<Playable*>* soundsList);
    void notify();

    void clientInit();
    bool serverInit();

    void addToRecord();

    Executor();

    static Executor* _instance;

    list<Playable*> _soundsList;
    stack<char> _availableSounds;

    list<tuple<int,function<void(const AudioBuffer&)>>> _observerList;
    AudioFile<float> _record;

    bool _isServerRunning = false;
};

}
#endif // EXECUTOR_H
