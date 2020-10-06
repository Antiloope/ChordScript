#ifndef OPERATORLINKEDVALUE_H
#define OPERATORLINKEDVALUE_H

#include "linkedvalue.h"

namespace CS {

class OperatorLinkedValue : public LinkedValue {
public:
    OperatorLinkedValue(size_t);
    void load(std::list<TerminalExpression*>*) override;
    void load(char);
    LiteralValue * getValue() const override;
private:
    char _operator;
};

}

#endif // OPERATORLINKEDVALUE_H
