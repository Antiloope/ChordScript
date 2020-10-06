#ifndef PROGRAMEXPRESSION_H
#define PROGRAMEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class InstructionExpression;

class ProgramExpression : public NonTerminalExpression {
public:
    ProgramExpression(size_t);
    ProgramExpression();
    ~ProgramExpression();
    void load(std::list<TerminalExpression*>*) override;
    void interpret() override;
    void interpret(std::list<TerminalExpression*>);
private:
    std::list<InstructionExpression*> _instructionsList;
};

}

#endif // PROGRAMEXPRESSION_H
