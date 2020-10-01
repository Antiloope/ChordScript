#ifndef NUMBERLINKEDVALUE_H
#define NUMBERLINKEDVALUE_H

#include "linkedvalue.h"

namespace CS {

class NumberLinkedValue : public LinkedValue {
public:
    NumberLinkedValue(size_t);
    void load(std::list<TerminalExpression*>*) override;
    LiteralValue * getValue() const override;
private:
    double _value;
};

}
#endif // NUMBERLINKEDVALUE_H
