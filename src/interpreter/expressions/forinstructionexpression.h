#ifndef FORINSTRUCTIONEXPRESSION_H
#define FORINSTRUCTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

// Defined in linkedvalue.h
class LinkedValue;
// Defined in assignationexpression.h
class AssignationExpression;

/**
 * @brief This class is the representaion of a for statement.
 * It holds the condition and the body
 */
class ForInstructionExpression : public NonTerminalExpression {
public:
    ForInstructionExpression(size_t codeReference);
    ~ForInstructionExpression();
    /**
     * @brief It loads the elements of the for: the assignation, condition,
     * iteration and code to loop. All values are LinkedValues at this moment.
     * @param terminalExpressionsList A list of tokens
     */
    void load(std::list<TerminalExpression*>* terminalExpressionsList) override;
    /**
     * @brief It iterates over the loop.
     */
    void interpret() override;
private:
    size_t _context;
    size_t _runningContext;
    AssignationExpression* _assignation = nullptr;
    LinkedValue* _booleanOperation = nullptr;
    AssignationExpression* _endAssignation = nullptr;
    ProgramExpression* _function = nullptr;
};

}

#endif // FORINSTRUCTIONEXPRESSION_H
