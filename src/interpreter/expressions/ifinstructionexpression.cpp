#include "ifinstructionexpression.h"
#include "terminalexpression.h"
#include "interpreter/context.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/values/linkedvalue.h"
#include "interpreter/values/literalvalue.h"

using namespace CS;
using namespace std;

IfInstructionExpression::IfInstructionExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void IfInstructionExpression::load(
    list<TerminalExpression*>* terminalExpressionsList) {

    TerminalExpression* aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::If) )
        throw SyntaxException("Expected 'if'",aux->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected if arguments",aux->getCodeReference() );

    delete aux;
    aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::OpenParenthesis) )
        throw SyntaxException("Expected (",aux->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected if arguments",aux->getCodeReference() );

    delete aux;
    aux = terminalExpressionsList->front();

    _condition = new BooleanOperationLinkedValue(aux->getCodeReference());
    _condition->load(terminalExpressionsList);

    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected )",this->getCodeReference() );

    aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::CloseParenthesis) )
        throw SyntaxException("Expected )",aux->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected {",aux->getCodeReference() );

    delete aux;
    aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::OpenBrace) )
        throw SyntaxException("Expected {",aux->getCodeReference());


    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected another symbol",aux->getCodeReference() );

    delete aux;
    aux = terminalExpressionsList->front();

    Context* ctx = Context::getInstance();

    _context = ctx->newScope();

    _function = new ProgramExpression(aux->getCodeReference());
    _function->load(terminalExpressionsList);

    ctx->returnScope();

    if( terminalExpressionsList->empty() )
        throw SyntaxException("Expected }",this->getCodeReference() );

    aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::CloseBrace) )
        throw SyntaxException("Expected }",aux->getCodeReference());

    terminalExpressionsList->pop_front();
    delete aux;

    if(
        terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::Else) )
    {
        _elseContext = 0;
        _elseFunction = nullptr;
    }
    else
    {
        delete terminalExpressionsList->front();
        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty() )
            throw SyntaxException("Expected {",aux->getCodeReference() );

        aux = terminalExpressionsList->front();

        if( aux->getType() != cCast(TerminalExpressionType::OpenBrace) )
            throw SyntaxException("Expected {",aux->getCodeReference());

        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty())
            throw SyntaxException("Expected another symbol",aux->getCodeReference() );

        delete aux;
        aux = terminalExpressionsList->front();

        _elseContext = ctx->newScope();

        _elseFunction = new ProgramExpression(aux->getCodeReference());
        _elseFunction->load(terminalExpressionsList);

        ctx->returnScope();

        if( terminalExpressionsList->empty() )
            throw SyntaxException("Expected }",this->getCodeReference() );

        aux = terminalExpressionsList->front();

        if( aux->getType() != cCast(TerminalExpressionType::CloseBrace) )
            throw SyntaxException("Expected }",aux->getCodeReference());

        terminalExpressionsList->pop_front();
        delete aux;
    }
}

IfInstructionExpression::~IfInstructionExpression(){
    if( _function )
        delete _function;
    if( _elseFunction )
        delete _elseFunction;
    if( _condition )
        delete _condition;

    if( _runningContext )
        Context::getInstance()->removeScope(_runningContext);
    if( _runningElseContext )
        Context::getInstance()->removeScope(_runningElseContext);
    if( _context )
        Context::getInstance()->removeScope(_context);
    if( _elseContext )
        Context::getInstance()->removeScope(_elseContext);
}

void IfInstructionExpression::interpret(){
    Context::getInstance()->setReturnValue(nullptr);
    if( *(bool*)_condition->getValue()->getValue() )
    {
        _runningContext = Context::getInstance()->switchScope(_context);
        scope_index tmp = _runningContext;
        if( !_runningContext )
            throw SemanticException("Invalid switch to unknown context",this->getCodeReference());

        _function->interpret();

        Context::getInstance()->removeScope(tmp);
    }
    else
        if( _elseFunction )
        {
            _runningElseContext = Context::getInstance()->switchScope(_elseContext);
            scope_index tmp = _runningElseContext;
            if( !_runningElseContext )
                throw SemanticException("Invalid switch to unknown context",this->getCodeReference());

            _elseFunction->interpret();

            Context::getInstance()->removeScope(tmp);
        }
}
