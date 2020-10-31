#ifndef EOEINSTRUCTIONEXPRESSION_H
#define EOEINSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {
/**
 * @brief This class only remove the EOE semicolon
 */
class EOEInstructionExpression : public NonTerminalExpression {
public:
    EOEInstructionExpression(size_t codeReference);
    ~EOEInstructionExpression();
    void load(std::list<TerminalExpression*>* terminalExpressionsList) override;
    void interpret() override;
};

}

#endif // EOEINSTRUCTIONEXPRESSION_H
