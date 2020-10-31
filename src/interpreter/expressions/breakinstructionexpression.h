#ifndef BREAKINSTRUCTIONEXPRESSION_H
#define BREAKINSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

/**
 * @brief This class represents the break inside a for loop
 */
class BreakInstructionExpression : public NonTerminalExpression {
public:
    BreakInstructionExpression(size_t codeReference);
    ~BreakInstructionExpression();
    void load(std::list<TerminalExpression*>* terminalExpressionsList) override;
    void interpret() override;
};

}
#endif // BREAKINSTRUCTIONEXPRESSION_H
