#include "operatorlinkedvalue.h"
#include "literalvalue.h"

using namespace CS;
using namespace std;

OperatorLinkedValue::OperatorLinkedValue(size_t codeReference) :
    LinkedValue(codeReference) {}

void OperatorLinkedValue::load(
    list<TerminalExpression*>* terminalExpressionsList
    ) {
    // NOT IMPLEMENTED
    terminalExpressionsList->pop_front();
}

void OperatorLinkedValue::load(char op) {
    _operator = op;
}

LiteralValue * OperatorLinkedValue::getValue() const {
    return new OperatorLiteralValue(_operator);
}
