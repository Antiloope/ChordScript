#ifndef LITERALVALUE_H
#define LITERALVALUE_H

#include "value.h"

namespace CS {

class LiteralValue : public Value {
public:
    LiteralValue(DataTypesId);
    LiteralValue(const LiteralValue&);
    virtual ~LiteralValue();
    void* getValue() const { return _value; }
    virtual LiteralValue* clone() = 0;
protected:
    void* _value;
};

}

#include "argumentliteralvalue.h"
#include "arrayliteralvalue.h"
#include "booleanliteralvalue.h"
#include "nullliteralvalue.h"
#include "numberliteralvalue.h"
#include "operatorliteralvalue.h"
#include "sampleliteralvalue.h"
#include "soundliteralvalue.h"
#include "stringliteralvalue.h"

#endif // LITERALVALUE_H
