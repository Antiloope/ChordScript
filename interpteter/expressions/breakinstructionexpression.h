#ifndef BREAKINSTRUCTIONEXPRESSION_H
#define BREAKINSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

class BreakInstructionExpression : public NonTerminalExpression {
public:
    BreakInstructionExpression(size_t);
    ~BreakInstructionExpression();
    void load(std::list<TerminalExpression*>*) override;
    void interpret() override;
};

}
#endif // BREAKINSTRUCTIONEXPRESSION_H
