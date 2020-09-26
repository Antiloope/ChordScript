#ifndef EXECUTORINTERFACE_H
#define EXECUTORINTERFACE_H

#include <vector>
#include <functional>

using namespace std;

namespace CS {

typedef vector<vector<float>> AudioBuffer;

class Playable;

class ExecutorInterface
{
public:
    static AudioBuffer getBuffer(double);
    static unsigned int getSampleRate();
    static char getSoundId();
    static void addSound(Playable*);
    static void removeSound(Playable*);
    static void removeAllSounds();
    static void closeAll();
    static void startRecording();
    static void stopRecording();
    static int addObserver(function<void(const AudioBuffer&)>);
    static void removeObserver(int);
private:
    ExecutorInterface();
};

}

#endif // EXECUTORINTERFACE_H
