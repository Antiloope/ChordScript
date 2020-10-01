#ifndef NONTERMINALEXPRESSION_H
#define NONTERMINALEXPRESSION_H

#include "expression.h"

namespace CS {

class TerminalExpression;

/**
 * @brief Base clas for non terminal expressions.
 * Define two pure virtual methods (load and interpret)
 */
class NonTerminalExpression : public Expression {
public:
    NonTerminalExpression();
    NonTerminalExpression(size_t);
    virtual ~NonTerminalExpression();
    /**
     * @brief This method load internal state based on list of expressions
     * recived by argument.
     * @param terminalExpressionsList
     */
    virtual void load(std::list<TerminalExpression*>*) = 0;
    /**
     * @brief Execute the expression based on internal stored data
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
