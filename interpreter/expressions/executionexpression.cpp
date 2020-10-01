#include "executionexpression.h"
#include "interpteter/context.h"
#include "terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpteter/values/arraylinkedvalue.h"
#include "interpteter/values/literalvalue.h"
#include <memory>

using namespace CS;
using namespace std;

ExecutionExpression::ExecutionExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void ExecutionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    if ( tmp->getType() != cCast(TerminalExpressionType::Name) )
        throw SyntaxException("Expected a name", tmp->getCodeReference());

    _name = ((NameExpression*)tmp)->getName();

    if ( !Context::getInstance()->isValidName(_name) )
        throw SyntaxException("Invalid name",tmp->getCodeReference());

    if (
        !Context::getInstance()->nameExist(_name) &&
        !Context::getInstance()->functionNameExist(_name)
        )
        throw SemanticException("Unrecognized name",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if ( terminalExpressionsList->empty() )
        throw SyntaxException("Expected arguments",tmp->getCodeReference());
    tmp = terminalExpressionsList->front();

    if ( tmp->getType() == cCast(TerminalExpressionType::OpenParenthesis) )
    {
        ArrayLinkedValue* aux = new ArrayLinkedValue(tmp->getCodeReference());
        _methodsList.push_back(tuple<string,ArrayLinkedValue*>(_name,aux));
        aux->load(terminalExpressionsList);
    }
    bool isValidMethod = true;
    while( isValidMethod )
    {
        if( tmp->getType() == cCast(TerminalExpressionType::MemberAccess) )
        {
            terminalExpressionsList->pop_front();
            if ( terminalExpressionsList->empty() )
                throw SyntaxException("Expected arguments",tmp->getCodeReference());
            tmp = terminalExpressionsList->front();

            if ( tmp->getType() != cCast(TerminalExpressionType::Name) )
                throw SyntaxException("Expected a name", tmp->getCodeReference());

            string name = ((NameExpression*)tmp)->getName();

            terminalExpressionsList->pop_front();
            if ( terminalExpressionsList->empty() )
                throw SyntaxException("Expected (",tmp->getCodeReference());
            tmp = terminalExpressionsList->front();

            if ( tmp->getType() != cCast(TerminalExpressionType::OpenParenthesis) )
                throw SyntaxException("Expected (",tmp->getCodeReference());

            ArrayLinkedValue* aux = new ArrayLinkedValue(tmp->getCodeReference());
            _methodsList.push_back(tuple<string,ArrayLinkedValue*>(name,aux));
            aux->load(terminalExpressionsList);
        }
        else
        {
            isValidMethod = false;
            if (
                !terminalExpressionsList->front() ||
                terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE)
                )
                throw SyntaxException("Expected ;",tmp->getCodeReference());
        }
    }
}

ExecutionExpression::~ExecutionExpression(){
    while( !_methodsList.empty() )
    {
        tuple<string,ArrayLinkedValue*> tmp = _methodsList.front();
        delete get<1>(tmp);
        _methodsList.pop_front();
    }
}

void ExecutionExpression::interpret() {
    if( _name == get<0>(_methodsList.front()) )
    {
        if( !Context::getInstance()->executeFunction(_name,get<1>(_methodsList.front())->getValue()) )
            throw SemanticException("Unknown function name", this->getCodeReference());
    }
    else
        for( auto method : _methodsList )
        {
            unique_ptr<LiteralValue> args = unique_ptr<LiteralValue>(get<1>(method)->getValue());
            if( !Context::getInstance()->executeMethod(_name,get<0>(method),args.get()) )
                throw SyntaxException("Unknown method name",get<1>(method)->getCodeReference());
        }

    Context::getInstance()->setReturnValue(nullptr);
}
