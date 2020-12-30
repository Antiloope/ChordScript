#ifndef INTERACTIVEINTERPRETER_H
#define INTERACTIVEINTERPRETER_H

#include "interpreter.h"

namespace CS {

class InteractiveInterpreter : public Interpreter
{
public:
    InteractiveInterpreter();
    int run() override;
};

}

#endif // INTERACTIVEINTERPRETER_H
