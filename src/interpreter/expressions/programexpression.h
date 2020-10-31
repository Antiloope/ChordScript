#ifndef PROGRAMEXPRESSION_H
#define PROGRAMEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

// Defined on instructionexpression.h
class InstructionExpression;

/**
 * @brief It holds a list of instructions to be interpreted
 */
class ProgramExpression : public NonTerminalExpression {
public:
    /**
     * @brief Default constructor.
     */
    ProgramExpression();
    /**
     * @brief Constructor to use in functions and if/for bodies.
     * @param codeReference
     */
    ProgramExpression(size_t codeReference);
    /**
     * @brief Default destructor. Free list of instructions
     */
    ~ProgramExpression();

    /**
     * @brief It generates instructions based on tokens received.
     * This method remove processed expressions from the list.
     * @param terminalExpressionsList A list of tokens.
     */
    void load(std::list<TerminalExpression*>* terminalExpressionsList) override;

    /**
     * @brief It interpret all instructions previously loaded in the program.
     */
    void interpret() override;
    /**
     * @brief It generates instructions based on the received expressions and
     * interprets them immediately afterwards.
     * @param terminalExpressionsList A list of tokens
     */
    void interpret(std::list<TerminalExpression*>* terminalExpressionsList);

private:
    std::list<InstructionExpression*> _instructionsList;
};

}

#endif // PROGRAMEXPRESSION_H
