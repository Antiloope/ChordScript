#ifndef EXECUTORINTERFACE_H
#define EXECUTORINTERFACE_H

#include <vector>
#include <functional>
#include <string>

namespace CS {

typedef std::vector<std::vector<float>> AudioBuffer;

class Playable;

class ExecutorInterface
{
public:
    static AudioBuffer getBuffer(double);
    static unsigned int getSampleRate();
    static void init();
    static char getSoundId();
    static void addSound(Playable*,std::string);
    static void removeSound(std::string);
    static void removeAllSounds();
    static void closeAll();
    static void startRecording();
    static void stopRecording();
    static int addObserver(std::function<void(const AudioBuffer&)>);
    static void removeObserver(int);
    static void startServer();
    static void restartServer();
    static void killServer();
private:
    ExecutorInterface();
};

}

#endif // EXECUTORINTERFACE_H
