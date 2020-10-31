#include "executionexpression.h"
#include "interpreter/context.h"
#include "terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/values/arraylinkedvalue.h"
#include "interpreter/values/literalvalue.h"
#include <memory>

using namespace CS;
using namespace std;

ExecutionExpression::ExecutionExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void ExecutionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    auto aux = terminalExpressionsList->front();
    auto codeReference = aux->getCodeReference();

    Context* ctx = Context::getInstance();

    if( aux->getType() != cCast(TerminalExpressionType::Name) )
        throw SyntaxException("Expected a name", codeReference);

    _name = ((NameExpression*)aux)->getName();

    if( !ctx->isValidName(_name) )
        throw SyntaxException("Invalid name",codeReference);

    if(
        !ctx->nameExist(_name) &&
        !ctx->functionNameExist(_name) )
        throw SemanticException("Unrecognized name",codeReference);

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty() )
        throw SyntaxException("Expected arguments",aux->getCodeReference());

    delete aux;
    aux = terminalExpressionsList->front();

    if( aux->getType() == cCast(TerminalExpressionType::OpenParenthesis) )
    {
        ArrayLinkedValue* args = new ArrayLinkedValue(aux->getCodeReference());
        _methodsList.push_back(tuple<string,ArrayLinkedValue*>(_name,args));
        args->load(terminalExpressionsList);
    }
    bool isValidMethod = true;
    while( isValidMethod )
    {
        if( aux->getType() == cCast(TerminalExpressionType::MemberAccess) )
        {
            terminalExpressionsList->pop_front();
            if ( terminalExpressionsList->empty() )
                throw SyntaxException("Expected arguments after .",aux->getCodeReference());

            delete aux;
            aux = terminalExpressionsList->front();

            if ( aux->getType() != cCast(TerminalExpressionType::Name) )
                throw SyntaxException("Expected a method name", aux->getCodeReference());

            string name = ((NameExpression*)aux)->getName();

            terminalExpressionsList->pop_front();
            if ( terminalExpressionsList->empty() )
                throw SyntaxException("Expected (",aux->getCodeReference());

            delete aux;
            aux = terminalExpressionsList->front();

            if ( aux->getType() != cCast(TerminalExpressionType::OpenParenthesis) )
                throw SyntaxException("Expected (",aux->getCodeReference());

            ArrayLinkedValue* args = new ArrayLinkedValue(aux->getCodeReference());
            _methodsList.push_back(tuple<string,ArrayLinkedValue*>(name,args));
            args->load(terminalExpressionsList);
        }
        else
        {
            isValidMethod = false;
            if (
                !terminalExpressionsList->front() ||
                terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE) )
                throw SyntaxException("Expected ;",aux->getCodeReference());
        }
    }
}

ExecutionExpression::~ExecutionExpression() {
    while( !_methodsList.empty() )
    {
        tuple<string,ArrayLinkedValue*> tmp = _methodsList.front();
        delete get<1>(tmp);
        _methodsList.pop_front();
    }
}

void ExecutionExpression::interpret() {
    Context* ctx = Context::getInstance();
    if( _name == get<0>(_methodsList.front()) )
    {
        try
        {
            if( !ctx->executeFunction(_name,get<1>(_methodsList.front())->getValue()) )
                throw SemanticException("Unknown function name", this->getCodeReference());
        }
        catch( Exception& e )
        {
            throw SemanticException(e.what(),this->getCodeReference());
        }
    }
    else
        for( auto method : _methodsList )
        {
            unique_ptr<LiteralValue> args = unique_ptr<LiteralValue>(get<1>(method)->getValue());

            try
            {
                if( !ctx->executeMethod(_name,get<0>(method),args.get()) )
                    throw SyntaxException("Unknown method name",get<1>(method)->getCodeReference());
            }
            catch( Exception& e )
            {
                throw SemanticException(e.what(),this->getCodeReference()+1);
            }
        }

    ctx->setReturnValue(nullptr);
}
