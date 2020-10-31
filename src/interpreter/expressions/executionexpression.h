#ifndef EXECUTIONEXPRESSION_H
#define EXECUTIONEXPRESSION_H

#include "nonterminalexpression.h"

namespace CS {

// Defined in arraylinkedvalue.h
class ArrayLinkedValue;

typedef std::tuple<std::string,ArrayLinkedValue*> method_def;

/**
 * @brief It represents an execution expression.
 * A funcion call or methods calls.
 */
class ExecutionExpression : public NonTerminalExpression {
public:
    ExecutionExpression(size_t codeReference);
    ~ExecutionExpression();
    /**
     * @brief It detects if there are a function call or methods calls.
     * Store the function/variable name and a list of methods if its the case.
     * @param terminalExpressionsList A list of tokens
     */
    void load(std::list<TerminalExpression*>* terminalExpressionsList) override;
    void interpret() override;
private:
    std::string _name;
    std::list<method_def> _methodsList;
};

}

#endif // EXECUTIONEXPRESSION_H
