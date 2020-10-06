#include "eoeinstructionexpression.h"

using namespace CS;
using namespace std;

EOEInstructionExpression::EOEInstructionExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void EOEInstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    terminalExpressionsList->pop_front();
}

EOEInstructionExpression::~EOEInstructionExpression() {}

void EOEInstructionExpression::interpret() {}
