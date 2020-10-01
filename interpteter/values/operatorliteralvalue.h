#ifndef OPERATORLITERALVALUE_H
#define OPERATORLITERALVALUE_H

#include "literalvalue.h"

namespace CS {

class OperatorLiteralValue : public LiteralValue {
public:
    OperatorLiteralValue(char);
    LiteralValue* clone();
    ~OperatorLiteralValue();
private:
    char _operator;
};

}


#endif // OPERATORLITERALVALUE_H
