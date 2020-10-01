#ifndef ARGUMENTLITERALVALUE_H
#define ARGUMENTLITERALVALUE_H

#include <list>
#include "literalvalue.h"

namespace CS {

class ArgumentLiteralValue : public LiteralValue {
public:
    ArgumentLiteralValue(std::list<LiteralValue*>);
    LiteralValue* clone();
    ~ArgumentLiteralValue();
private:
    std::list<LiteralValue*> _literalValuesList;
};

}

#endif // ARGUMENTLITERALVALUE_H
