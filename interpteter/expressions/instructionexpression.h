#ifndef INSTRUCTIONEXPRESSION_H
#define INSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class InstructionExpression : public NonTerminalExpression {
public:
    InstructionExpression(size_t);
    InstructionExpression(const InstructionExpression&);
    ~InstructionExpression();
    void load(std::list<TerminalExpression*>*) override;
    void interpret() override;
private:
    NonTerminalExpression* _instruction = nullptr;
};

}

#endif // INSTRUCTIONEXPRESSION_H
