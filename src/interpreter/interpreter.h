#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

namespace CS {
/**
 * @brief Holds a sigle static method that interpret the code
 */
class Interpreter
{
public:
    /**
     * @brief Interpret the source code and thow a CS::Exception if there are any
     * error in the code.
     * @param sourceCode Plain text with the code to interpret.
     */
    static void interpret(const std::string sourceCode);
};

}

#endif // INTERPRETER_H
