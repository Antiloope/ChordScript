#ifndef ARRAYLINKEDVALUE_H
#define ARRAYLINKEDVALUE_H

#include "linkedvalue.h"

namespace CS {

class ArrayLinkedValue : public LinkedValue {
public:
    ArrayLinkedValue(size_t);
    ~ArrayLinkedValue();
    void load(std::list<TerminalExpression*>*) override;
    LiteralValue * getValue() const override;
private:
    std::list<LinkedValue*> _linkedValuesList;
    DataTypesId _type;
};

}

#endif // ARRAYLINKEDVALUE_H
