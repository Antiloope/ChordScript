#ifndef STRINGLITERALVALUE_H
#define STRINGLITERALVALUE_H

#include <list>
#include "literalvalue.h"

namespace CS {

class TerminalExpression;

class StringLiteralValue : public LiteralValue {
public:
    StringLiteralValue(std::string);
    LiteralValue* clone();
    ~StringLiteralValue();
private:
    std::string _text;
};

}

#endif // STRINGLITERALVALUE_H
