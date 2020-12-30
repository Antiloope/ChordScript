#ifndef PIPEINTERPRETER_H
#define PIPEINTERPRETER_H

#include "interpreter.h"

namespace CS {

class PipeInterpreter : public Interpreter
{
public:
    PipeInterpreter();
    int run() override;

    static std::string interpret(std::string source);
    static int stopServer();
    static int startServer();
private:
    static bool isRunning();
};

}

#endif // PIPEINTERPRETER_H
