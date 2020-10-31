#ifndef RETURNINSTRUCTIONEXPRESSION_H
#define RETURNINSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

// Defined in linkedvalue.h
class LinkedValue;

/**
 * @brief This class store the return value of a function
 */
class ReturnInstructionExpression : public NonTerminalExpression {
public:
    ReturnInstructionExpression(size_t codeReference);
    ~ReturnInstructionExpression();
    /**
     * @brief It loads the return value as a LinkedValue.
     * @param terminalExpressionsList A list of tokens
     */
    void load(std::list<TerminalExpression*>* terminalExpressionsList) override;
    /**
     * @brief It gets the LiteralValue and set the return value in Context
     */
    void interpret() override;
private:
    LinkedValue* _returnValue = nullptr;
};

}

#endif // RETURNINSTRUCTIONEXPRESSION_H
