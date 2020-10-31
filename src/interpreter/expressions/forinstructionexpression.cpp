#include "forinstructionexpression.h"
#include "interpreter/context.h"
#include "terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/values/booleanoperationlinkedvalue.h"
#include <memory>
#include "interpreter/values/literalvalue.h"

using namespace CS;
using namespace std;

ForInstructionExpression::ForInstructionExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void ForInstructionExpression::load(
    list<TerminalExpression*>* terminalExpressionsList) {

    TerminalExpression* aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::For) )
        throw SyntaxException("Expected 'for'",aux->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected for arguments",aux->getCodeReference() );

    delete aux;
    aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::OpenParenthesis) )
        throw SyntaxException("Expected (",aux->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected for arguments",aux->getCodeReference() );

    delete aux;
    aux = terminalExpressionsList->front();

    Context* ctx = Context::getInstance();

    _context = ctx->newScope();

    _assignation = new AssignationExpression(aux->getCodeReference());
    _assignation->load(terminalExpressionsList);

    if(
        terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE) )
        throw SyntaxException("Expected ;",this->getCodeReference() );

    delete terminalExpressionsList->front();
    terminalExpressionsList->pop_front();
    aux = terminalExpressionsList->front();

    _booleanOperation = new BooleanOperationLinkedValue(aux->getCodeReference());
    _booleanOperation->load(terminalExpressionsList);

    if(
        terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE) )
        throw SyntaxException("Expected ;",this->getCodeReference() );

    delete terminalExpressionsList->front();
    terminalExpressionsList->pop_front();
    aux = terminalExpressionsList->front();

    _endAssignation = new AssignationExpression(aux->getCodeReference());
    _endAssignation->load(terminalExpressionsList);

    if(
        terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::CloseParenthesis) )
        throw SyntaxException("Expected )",this->getCodeReference() );

    delete terminalExpressionsList->front();
    terminalExpressionsList->pop_front();
    aux = terminalExpressionsList->front();

    if(
        terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::OpenBrace) )
        throw SyntaxException("Expected {",aux->getCodeReference() );

    delete terminalExpressionsList->front();
    terminalExpressionsList->pop_front();
    aux = terminalExpressionsList->front();

    _function = new ProgramExpression(aux->getCodeReference());
    _function->load(terminalExpressionsList);

    if( terminalExpressionsList->empty() )
        throw SyntaxException("Expected }",aux->getCodeReference() );

    aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::CloseBrace) )
        throw SyntaxException("Expected }",aux->getCodeReference());

    terminalExpressionsList->pop_front();
    delete aux;

    ctx->returnScope();
}

ForInstructionExpression::~ForInstructionExpression() {
    if( _function )
        delete _function;
    if( _endAssignation )
        delete _endAssignation;
    if( _assignation )
        delete _assignation;
    if( _booleanOperation )
        delete _booleanOperation;

    Context::getInstance()->removeScope(_runningContext);
    Context::getInstance()->removeScope(_context);
}

void ForInstructionExpression::interpret() {
    Context* ctx = Context::getInstance();
    _runningContext = ctx->switchScope(_context);

    scope_index tmp = _runningContext;

    if( !_runningContext )
        throw SemanticException("Invalid switch to unknown context",this->getCodeReference());

    ctx->setReturnValue(nullptr);

    _assignation->interpret();
    auto booleanCondition = unique_ptr<LiteralValue>(_booleanOperation->getValue());

    ctx->setReturnValue(nullptr);
    while( *(bool*)booleanCondition->getValue() )
    {
        _function->interpret();
        if( ctx->existReturnValue() )
            break;

        _endAssignation->interpret();
        booleanCondition = unique_ptr<LiteralValue>(_booleanOperation->getValue());
    }
    ctx->removeScope(tmp);
}
