#ifndef NUMBERLITERALVALUE_H
#define NUMBERLITERALVALUE_H

#include "literalvalue.h"

namespace CS {

class NumberLiteralValue : public LiteralValue {
public:
    NumberLiteralValue(double);
    LiteralValue* clone();
    ~NumberLiteralValue();
private:
    double _number;
};

}

#endif // NUMBERLITERALVALUE_H
