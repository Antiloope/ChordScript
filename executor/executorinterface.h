#ifndef EXECUTORINTERFACE_H
#define EXECUTORINTERFACE_H

#include <vector>

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
};

}

#endif // EXECUTORINTERFACE_H
