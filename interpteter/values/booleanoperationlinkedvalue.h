#ifndef BOOLEANOPERATIONLINKEDVALUE_H
#define BOOLEANOPERATIONLINKEDVALUE_H

#include "linkedvalue.h"

namespace CS {

enum class BooleanSymbols : char {
    OpenParenthesis = 'a',
    And,
    Or,
    Equal,
    Negation,
    NotEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual
};

class BooleanOperationLinkedValue : public LinkedValue {
public:
    BooleanOperationLinkedValue(size_t);
    ~BooleanOperationLinkedValue();
    void load(std::list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    std::list<LinkedValue*> _linkedValuesList;
};

}

#endif // BOOLEANOPERATIONLINKEDVALUE_H
