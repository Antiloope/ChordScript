#include "eoeinstructionexpression.h"
#include "terminalexpression.h"

using namespace CS;
using namespace std;

EOEInstructionExpression::EOEInstructionExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void EOEInstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    delete terminalExpressionsList->front();
    terminalExpressionsList->pop_front();
}

EOEInstructionExpression::~EOEInstructionExpression() {}

void EOEInstructionExpression::interpret() {}
