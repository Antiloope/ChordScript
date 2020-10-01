#include "forinstructionexpression.h"
#include "interpreter/context.h"
#include "terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/values/booleanoperationlinkedvalue.h"
#include <memory>
#include "interpreter/values/literalvalue.h"

using namespace CS;
using namespace std;

ForInstructionExpression::ForInstructionExpression(size_t codeReference) : NonTerminalExpression(codeReference) {}

void ForInstructionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    if( tmp->getType() != cCast(TerminalExpressionType::For) )
        throw SyntaxException("Expected for",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected for arguments",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    if( tmp->getType() != cCast(TerminalExpressionType::OpenParenthesis) )
        throw SyntaxException("Expected (",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected for arguments",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    Context* ctx = Context::getInstance();

    _context = ctx->newScope();

    _assignation = new AssignationExpression(tmp->getCodeReference());
    _assignation->load(terminalExpressionsList);

    if(
        terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE)
        )
        throw SyntaxException("Expected ;",tmp->getCodeReference() );

    terminalExpressionsList->pop_front();
    tmp = terminalExpressionsList->front();

    _booleanOperation = new BooleanOperationLinkedValue(tmp->getCodeReference());
    _booleanOperation->load(terminalExpressionsList);

    if(
        terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE)
        )
        throw SyntaxException("Expected ;",tmp->getCodeReference() );

    terminalExpressionsList->pop_front();
    tmp = terminalExpressionsList->front();

    _endAssignation = new AssignationExpression(tmp->getCodeReference());
    _endAssignation->load(terminalExpressionsList);

    if(
        terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::CloseParenthesis)
        )
        throw SyntaxException("Expected )",tmp->getCodeReference() );

    terminalExpressionsList->pop_front();
    tmp = terminalExpressionsList->front();

    if(
        terminalExpressionsList->empty() ||
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::OpenBrace)
        )
        throw SyntaxException("Expected {",tmp->getCodeReference() );

    terminalExpressionsList->pop_front();
    tmp = terminalExpressionsList->front();

    _function = new ProgramExpression(tmp->getCodeReference());
    _function->load(terminalExpressionsList);

    if( terminalExpressionsList->empty() )
        throw SyntaxException("Expected }",tmp->getCodeReference() );

    tmp = terminalExpressionsList->front();

    if( tmp->getType() != cCast(TerminalExpressionType::CloseBrace) )
        throw SyntaxException("Expected }",tmp->getCodeReference());

    terminalExpressionsList->pop_front();

    ctx->returnScope();
}

ForInstructionExpression::~ForInstructionExpression(){
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

void ForInstructionExpression::interpret(){
    _runningContext = Context::getInstance()->switchScope(_context);
    scope_index tmp = _runningContext;
    if( !_runningContext )
        throw SemanticException("Invalid switch to unknown context",this->getCodeReference());

    Context::getInstance()->setReturnValue(nullptr);
    _assignation->interpret();

    unique_ptr<LiteralValue> booleanCondition = unique_ptr<LiteralValue>(_booleanOperation->getValue());
    while(
        !Context::getInstance()->existReturnValue() &&
        *(bool*)booleanCondition->getValue() )
    {
        booleanCondition = unique_ptr<LiteralValue>(_booleanOperation->getValue());
        _function->interpret();
        _endAssignation->interpret();
    }
    Context::getInstance()->removeScope(tmp);
}
