#include "stringlinkedvalue.h"
#include "interpreter/expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "stringliteralvalue.h"

using namespace CS;
using namespace std;

StringLinkedValue::StringLinkedValue(size_t codeReference) : LinkedValue(codeReference) {}

void StringLinkedValue::load(
    list<TerminalExpression*>* terminalExpressionsList
    ) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(TerminalExpressionType::String) ) throw SyntaxException("Expected a string",tmp->getCodeReference());

    _text = ((StringExpression*)tmp)->getText();

    terminalExpressionsList->pop_front();
}

LiteralValue* StringLinkedValue::getValue() const {
    return new StringLiteralValue(_text);
}
