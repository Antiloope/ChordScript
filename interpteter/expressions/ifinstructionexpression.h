#ifndef IFINSTRUCTIONEXPRESSION_H
#define IFINSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class LinkedValue;
class ProgramExpression;

class IfInstructionExpression : public NonTerminalExpression {
public:
    IfInstructionExpression(size_t);
    ~IfInstructionExpression();
    void load(std::list<TerminalExpression*>*) override;
    void interpret() override;
private:
    size_t _context;
    size_t _runningContext;
    size_t _elseContext;
    size_t _runningElseContext;
    LinkedValue* _condition = nullptr;
    ProgramExpression* _function = nullptr;
    ProgramExpression* _elseFunction = nullptr;
};

}

#endif // IFINSTRUCTIONEXPRESSION_H
