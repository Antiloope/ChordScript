#ifndef NULLLITERALVALUE_H
#define NULLLITERALVALUE_H

#include "literalvalue.h"

namespace CS {

class NullLiteralValue : public LiteralValue {
public:
    NullLiteralValue();
    LiteralValue* clone();
    ~NullLiteralValue();
};

}

#endif // NULLLITERALVALUE_H
