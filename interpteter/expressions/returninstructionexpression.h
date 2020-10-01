#ifndef RETURNINSTRUCTIONEXPRESSION_H
#define RETURNINSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class LinkedValue;

class ReturnInstructionExpression : public NonTerminalExpression {
public:
    ReturnInstructionExpression(size_t);
    ~ReturnInstructionExpression();
    void load(std::list<TerminalExpression*>*) override;
    void interpret() override;
private:
    LinkedValue* _returnValue = nullptr;
};

}

#endif // RETURNINSTRUCTIONEXPRESSION_H
