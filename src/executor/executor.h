#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <list>
#include <stack>
#include <string>
#include "executor/buffer/buffer.h"
#include <functional>

namespace CS {

class Playable;

/**
 * @brief This class manages the interface with jack library.
 * It has methods to manage what sounds are played.
 */
class Executor
{
public:
    static Executor* getInstance();
    ~Executor();

    /**
     * @brief Add the sound passed as parameter into a queue,
     * waiting to be added later into the reproduction list.
     * @param newSound Poiter to a Sound to be played
     */
    void addSound(Playable*,std::string) const;
    /**
     * @brief This method must be called first of all.
     * It start jack client and connect porst to the jack server and
     * start a thread to handle queue and reproduction list sync
     */
    void init();

    void removeSound(Playable*,std::string) const;

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

    int addObserver(std::function<void(const AudioBuffer&)>);
    void removeObserver(int);
private:
    static void loadBuffer(std::list<std::tuple<Playable*,std::string>>* soundsList);
    void notify();

    void clientInit();
    bool serverInit();

    void addToRecord();

    Executor();

    static Executor* _instance;

    std::list<std::tuple<Playable*,std::string>> _soundsList;
    std::stack<char> _availableSounds;

    std::list<std::tuple<int,std::function<void(const AudioBuffer&)>>> _observerList;
    AudioFile<float> _record;

    bool _isServerRunning = false;
};

}
#endif // EXECUTOR_H
