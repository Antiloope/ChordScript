#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

using namespace std;


namespace CS {

class Interpreter
{
public:
    Interpreter();
    void interpret(const string sourceCode);
};

}

#endif // INTERPRETER_H
