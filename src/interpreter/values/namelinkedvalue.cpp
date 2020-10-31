#include "namelinkedvalue.h"
#include "interpreter/expressions/terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/values/literalvalue.h"
#include "interpreter/context.h"

using namespace CS;
using namespace std;

NameLinkedValue::NameLinkedValue(size_t codeReference) :
    LinkedValue(codeReference) {}

void NameLinkedValue::load(
    list<TerminalExpression*>* terminalExpressionsList
    ){
    if( terminalExpressionsList->empty() )
        throw SyntaxException("Expected a name");

    NameExpression* nameExpression = (NameExpression*)terminalExpressionsList->front();
    _name = nameExpression->getName();

    if( !Context::getInstance()->isValidName(_name) )
        throw SyntaxException("Invalid name",nameExpression->getCodeReference());

    if( !Context::getInstance()->nameExist(_name) )
        throw SemanticException("Unrecognized name",nameExpression->getCodeReference());

    terminalExpressionsList->pop_front();
}

LiteralValue* NameLinkedValue::getValue() const {
    LiteralValue* ret = Context::getInstance()->getVariableValue(_name);
    if( ret )
        return ret;

    return new NullLiteralValue();
}
