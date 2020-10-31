#ifndef DEFINITIONEXPRESSION_H
#define DEFINITIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {
/**
 * @brief This class interpret functions and variables definitions.
 */
class DefinitionExpression : public NonTerminalExpression {
public:
    DefinitionExpression(size_t codeReference);
    ~DefinitionExpression();
    /**
     * @brief It loads in Context the function
     * @param terminalExpressionsList A list of tokens
     */
    void load(std::list<TerminalExpression*>* terminalExpressionsList) override;
    /**
     * @brief Do nothing because function was loaded before
     */
    void interpret() override;
};

}

#endif // DEFINITIONEXPRESSION_H
