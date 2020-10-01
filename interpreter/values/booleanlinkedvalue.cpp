#include "booleanlinkedvalue.h"
#include "interpreter/expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/values/literalvalue.h"

using namespace CS;
using namespace std;

BooleanLinkedValue::BooleanLinkedValue(size_t codeReference) :
    LinkedValue(codeReference) {}

void BooleanLinkedValue::load(
    list<TerminalExpression*>* terminalExpressionsList
    ) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    if( tmp->getType() != cCast(TerminalExpressionType::Boolean) )
        throw SyntaxException("Expected a boolean value",tmp->getCodeReference());

    _value = ((BooleanExpression*)tmp)->getValue();

    terminalExpressionsList->pop_front();
}

LiteralValue * BooleanLinkedValue::getValue() const {
    return new BooleanLiteralValue(_value);
}
