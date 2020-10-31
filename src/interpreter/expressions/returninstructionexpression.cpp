#include "returninstructionexpression.h"
#include "interpreter/context.h"
#include "interpreter/values/linkedvalue.h"
#include "interpreter/values/literalvalue.h"
#include "terminalexpression.h"
#include "utils/Exceptions/exception.h"

using namespace CS;
using namespace std;

ReturnInstructionExpression::ReturnInstructionExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void ReturnInstructionExpression::load(
    list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::Return) )
        throw SyntaxException("Expected return", aux->getCodeReference());

    if( Context::getInstance()->getCurrentScope() == GLOBAL_SCOPE )
        throw SyntaxException("Return is not valid in global scope",aux->getCodeReference());

    terminalExpressionsList->pop_front();
    if ( terminalExpressionsList->empty() )
        throw SyntaxException("Expected a return value or ;",aux->getCodeReference());

    delete aux;
    aux = terminalExpressionsList->front();

    if ( aux->getType() == cCast(TerminalExpressionType::EOE) )
    {
        _returnValue = new NullLinkedValue(aux->getCodeReference());
        _returnValue->load(terminalExpressionsList);
    }
    else
    {
        _returnValue = LinkedValue::generateLinkedValue(terminalExpressionsList);
        aux = terminalExpressionsList->front();
        if ( aux->getType() != cCast(TerminalExpressionType::EOE) )
            throw SyntaxException("Expected ;",aux->getCodeReference());
    }
}

ReturnInstructionExpression::~ReturnInstructionExpression() {
    if( _returnValue )
        delete _returnValue;
}

void ReturnInstructionExpression::interpret() {
    auto ret = _returnValue->getValue();
    Context::getInstance()->setReturnValue(ret);
    if( ret )
        delete ret;
}
