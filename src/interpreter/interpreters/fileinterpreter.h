#ifndef FILEINTERPRETER_H
#define FILEINTERPRETER_H

#include "interpreter.h"

namespace CS {

class FileInterpreter : public Interpreter
{
public:
    FileInterpreter(const char* fileName);
    int run() override;
private:
    std::string _fileName;
};

}

#endif // FILEINTERPRETER_H
