#ifndef IFINSTRUCTIONEXPRESSION_H
#define IFINSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

// Defined in linkedvalue.h
class LinkedValue;
// Defined in programexpression.h
class ProgramExpression;

/**
 * @brief This class represents an if statement
 */
class IfInstructionExpression : public NonTerminalExpression {
public:
    IfInstructionExpression(size_t codeReference);
    ~IfInstructionExpression();
    /**
     * @brief It loads the if condition and the content of the if.
     * At this moment, all values are LinkedValues.
     * @param terminalExpressionsList A list of tokens
     */
    void load(std::list<TerminalExpression*>* terminalExpressionsList) override;
    /**
     * @brief It checks the condition and run the code in the if
     */
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
