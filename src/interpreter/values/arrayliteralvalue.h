#ifndef ARRAYLITERALVALUE_H
#define ARRAYLITERALVALUE_H

#include <list>
#include "literalvalue.h"

namespace CS {

class ArrayLiteralValue : public LiteralValue {
public:
    ArrayLiteralValue(std::list<LiteralValue*>);
    LiteralValue* clone();
    ~ArrayLiteralValue();
private:
    std::list<LiteralValue*> _literalValuesList;
};

}

#endif // ARRAYLITERALVALUE_H
