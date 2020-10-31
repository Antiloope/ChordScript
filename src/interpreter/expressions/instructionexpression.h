#ifndef INSTRUCTIONEXPRESSION_H
#define INSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

/**
 * @brief It is the most abstract NonTerminalExpression after a ProgramExpression.
 * Each instance of this class store a concrete instruction and has methods
 * to interpret that expression.
 */
class InstructionExpression : public NonTerminalExpression {
public:
    /**
     * @brief Constructor with a code reference
     * @param codeReference Number of the reference character
     */
    InstructionExpression(size_t codeReference);
    ~InstructionExpression();
    /**
     * @brief It creates a NonTerminalExpression based on tokens received as parameter
     * @param terminalExpressionsList A list of tokens
     */
    void load(std::list<TerminalExpression*>* terminalExpressionsList) override;
    /**
     * @brief Interpret the stored expression
     */
    void interpret() override;
private:
    NonTerminalExpression* _instruction = nullptr;
};

}

#endif // INSTRUCTIONEXPRESSION_H
