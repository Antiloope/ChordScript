#include "numberlinkedvalue.h"
#include "interpreter/expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/values/literalvalue.h"

using namespace CS;
using namespace std;

NumberLinkedValue::NumberLinkedValue(size_t codeReference) :
    LinkedValue(codeReference) {}

void NumberLinkedValue::load(
    list<TerminalExpression*>* terminalExpressionsList
    ) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(TerminalExpressionType::Numeric) )
        throw SyntaxException("Expected a numeric value",tmp->getCodeReference());

    _value = ((NumericExpression*)tmp)->getValue();

    terminalExpressionsList->pop_front();
}

LiteralValue* NumberLinkedValue::getValue() const {
    return new NumberLiteralValue(_value);
}
