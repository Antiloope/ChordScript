#ifndef NONTERMINALEXPRESSION_H
#define NONTERMINALEXPRESSION_H

#include "expression.h"
#include <list>

namespace CS {

// Defined in terminalexpression.h
class TerminalExpression;

/**
 * @brief Base class for non terminal expressions.
 * Define two pure virtual methods (load and interpret)
 */
class NonTerminalExpression : public Expression {
public:
    NonTerminalExpression();
    NonTerminalExpression(size_t codeReference);
    virtual ~NonTerminalExpression();
    /**
     * @brief It loads an internal state based on list of expressions
     * recived by argument.
     * @param terminalExpressionsList A list of tokens
     */
    virtual void load(std::list<TerminalExpression*>*) = 0;
    /**
     * @brief It executes the expression based on internal stored data
     */
    virtual void interpret() = 0;
};

}

#include "assignationexpression.h"
#include "breakinstructionexpression.h"
#include "definitionexpression.h"
#include "eoeinstructionexpression.h"
#include "executionexpression.h"
#include "instructionexpression.h"
#include "returninstructionexpression.h"
#include "programexpression.h"
#include "ifinstructionexpression.h"
#include "forinstructionexpression.h"

#endif // NONTERMINALEXPRESSION_H
