#ifndef EXECUTIONLINKEDVALUE_H
#define EXECUTIONLINKEDVALUE_H

#include "linkedvalue.h"

namespace CS {

class ArrayLinkedValue;

class ExecutionLinkedValue : public LinkedValue {
public:
    ExecutionLinkedValue(size_t);
    ~ExecutionLinkedValue();
    void load(std::list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    std::string _name;
    std::list<std::tuple<std::string,ArrayLinkedValue*>> _methodsList;
};

}

#endif // EXECUTIONLINKEDVALUE_H
