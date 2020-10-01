#include "nonterminalexpression.h"

using namespace CS;

NonTerminalExpression::NonTerminalExpression() : Expression() {}

NonTerminalExpression::NonTerminalExpression(size_t codeReference) :
    Expression(codeReference) {}

NonTerminalExpression::~NonTerminalExpression() {}
