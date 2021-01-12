#ifndef INTERACTIVEINTERPRETER_H
#define INTERACTIVEINTERPRETER_H

#include "interpreter.h"
#include <vector>

namespace CS {

/**
 * @brief This class represents the interface with the user.
 * It has methods to handle input and output.
 */
class Console {
public:
    void init();
    std::string getSourceCode();
    void showError(std::string sourceCode,int charReference, std::string description);
    void close();
private:
    bool validSource(std::string sourceCode);
    std::vector<std::string> oldLines;
};

class InteractiveInterpreter : public Interpreter
{
public:
    InteractiveInterpreter();
    int run() override;
private:
    Console _console;
};

}

#endif // INTERACTIVEINTERPRETER_H
