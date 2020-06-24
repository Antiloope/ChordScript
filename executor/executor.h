#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <list>
#include "buffer.h"

using namespace std;

namespace CS {

class Executor
{
public:
    static Executor* getInstance();
    ~Executor();
    // TODO: Configure parameters to init.
    void init();
private:
    Executor();
    static Executor* _instance;

    list<Sound*> _soundsList;
};

}
#endif // EXECUTOR_H
