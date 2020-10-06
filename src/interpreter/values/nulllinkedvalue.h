#ifndef NULLLINKEDVALUE_H
#define NULLLINKEDVALUE_H

#include "linkedvalue.h"

namespace CS {

class NullLinkedValue : public LinkedValue {
public:
    NullLinkedValue(size_t);
    void load(std::list<TerminalExpression*>*) override;
    LiteralValue * getValue() const override;
};

}

#endif // NULLLINKEDVALUE_H
