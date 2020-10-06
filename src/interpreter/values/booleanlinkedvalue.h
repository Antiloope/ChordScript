#ifndef BOOLEANLINKEDVALUE_H
#define BOOLEANLINKEDVALUE_H

#include "linkedvalue.h"

namespace CS {

class BooleanLinkedValue : public LinkedValue {
public:
    BooleanLinkedValue(size_t);
    void load(std::list<TerminalExpression*>*) override;
    LiteralValue * getValue() const override;
private:
    bool _value;
};

}

#endif // BOOLEANLINKEDVALUE_H
