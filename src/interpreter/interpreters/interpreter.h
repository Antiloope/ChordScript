#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

enum ReturnCodes : int
{
    SUCCESS = 0,
    NOTHING_TO_DO,
    FORK_ERROR,
    ANOTHER_INSTANCE_RUNNING,
    INVALID_FILE_NAME,
    CONFIG_ERROR,
    CODE_ERROR,
    CHILD_PROCESS_RETURN
};

namespace CS {

class Interpreter
{
public:
    virtual int run() = 0;

protected:
    /**
     * @brief Interpret the source code and thow a CS::Exception if there are any
     * error in the code.
     * @param sourceCode Plain text with the code to interpret.
     */
    static void interpret(const std::string sourceCode);
};

}

#include "fileinterpreter.h"
#include "interactiveinterpreter.h"
#include "pipeinterpreter.h"

#endif // INTERPRETER_H
