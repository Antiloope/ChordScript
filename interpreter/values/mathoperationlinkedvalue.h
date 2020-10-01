#ifndef MATHOPERATIONLINKEDVALUE_H
#define MATHOPERATIONLINKEDVALUE_H

#include "linkedvalue.h"

namespace CS {

enum class MathSymbols : char{
    OpenParenthesis = 'a',
    Multiplication,
    Divition,
    Addition,
    Substraction
};

class MathOperationLinkedValue : public LinkedValue {
public:
    MathOperationLinkedValue(size_t);
    ~MathOperationLinkedValue();
    void load(std::list<TerminalExpression*>*) override;
    LiteralValue * getValue() const override;
private:
    std::list<LinkedValue*> _linkedValuesList;
};

}

#endif // MATHOPERATIONLINKEDVALUE_H
