#ifndef BOOLEANLITERALVALUE_H
#define BOOLEANLITERALVALUE_H

#include "literalvalue.h"

namespace CS {

class BooleanLiteralValue : public LiteralValue {
public:
    BooleanLiteralValue(bool);
    LiteralValue* clone();
    ~BooleanLiteralValue();
private:
    bool _boolean;
};

}


#endif // BOOLEANLITERALVALUE_H
