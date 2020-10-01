#include "functiondefinition.h"
#include "context.h"
#include "expressions/terminalexpression.h"
#include "expressions/nonterminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "values/literalvalue.h"

using namespace CS;
using namespace std;

ArgumentDefinition::ArgumentDefinition(DataTypesId dataType, string name) : _dataType(dataType),_name(name) {}

string ArgumentDefinition::getName() const {
    return _name;
}

DataTypesId ArgumentDefinition::getDataTypeId() const {
    return _dataType;
}

FunctionDefinition::FunctionDefinition() {}

void FunctionDefinition::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression *tmp = terminalExpressionsList->front();

    Context* ctx = Context::getInstance();

    if( ctx->getCurrentScope() != GLOBAL_SCOPE ) throw SemanticException("Functions must be defined on global scope",tmp->getCodeReference());

    _context = ctx->newScope();

    if( tmp->getType() != cCast(TerminalExpressionType::OpenParenthesis) ) throw SyntaxException("Expected (",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    DataTypesId dataType;

    bool isValidArgument = true;
    while( isValidArgument )
    {
        if ( tmp->getType() == cCast(TerminalExpressionType::Name) )
        {
            if ( !ctx->isDataType(DataType::getDataTypeId(((NameExpression*)tmp)->getName())) ) throw SyntaxException("Expected data type", tmp->getCodeReference());

            dataType = DataType::getDataTypeId(((NameExpression*)tmp)->getName());

            terminalExpressionsList->pop_front();
            if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
            tmp = terminalExpressionsList->front();

            if( tmp->getType() != cCast(TerminalExpressionType::Name) ) throw SyntaxException("Expected argument name",tmp->getCodeReference());

            if( !ctx->isValidName(((NameExpression*)tmp)->getName()) ) throw SyntaxException("Invalid name",tmp->getCodeReference());

            ctx->newVariable(((NameExpression*)tmp)->getName(),dataType);

            _argumentsDefinitionList.push_back(ArgumentDefinition(dataType,((NameExpression*)tmp)->getName()));

            terminalExpressionsList->pop_front();
            if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
            tmp = terminalExpressionsList->front();

            if( tmp->getType() == cCast(TerminalExpressionType::Separator) )
            {
                terminalExpressionsList->pop_front();
                if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                tmp = terminalExpressionsList->front();

                if( tmp->getType() == cCast(TerminalExpressionType::CloseParenthesis) )
                {
                    isValidArgument = false;

                    terminalExpressionsList->pop_front();
                    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                    tmp = terminalExpressionsList->front();
                }
            }
            else if( tmp->getType() == cCast(TerminalExpressionType::CloseParenthesis) )
            {
                isValidArgument = false;

                terminalExpressionsList->pop_front();
                if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                tmp = terminalExpressionsList->front();
            }
            else
            {
                throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
            }
        }
        else if( tmp->getType() == cCast(TerminalExpressionType::CloseParenthesis) )
        {
            isValidArgument = false;

            terminalExpressionsList->pop_front();
            if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
            tmp = terminalExpressionsList->front();
        }
        else
        {
            throw SyntaxException("Expected arguments", tmp->getCodeReference());
        }
    }

    if( tmp->getType() != cCast(TerminalExpressionType::OpenBrace) ) throw SyntaxException("Expected {",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    _function = new ProgramExpression(tmp->getCodeReference());
    _function->load(terminalExpressionsList);

    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected }",tmp->getCodeReference() );

    tmp = terminalExpressionsList->front();
    if ( tmp->getType() != cCast(TerminalExpressionType::CloseBrace) ) throw SyntaxException("Expected }",tmp->getCodeReference() );

    terminalExpressionsList->pop_front();

    ctx->returnScope();
}

void FunctionDefinition::interpret(LiteralValue* literalArgs) {
    if( literalArgs->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid arguments");

    list<LiteralValue*> args = *(list<LiteralValue*>*)literalArgs->getValue();

    if( args.size() != _argumentsDefinitionList.size() )
        throw SyntaxException("Invalid number of arguments");

    Context* ctx = Context::getInstance();
    _runningContext = ctx->switchScope(_context);
    scope_index tmp = _runningContext;

    for( auto argumentDefinition : _argumentsDefinitionList )
    {
        ctx->setVariableValue(argumentDefinition.getName(),args.front());
        args.pop_front();
    }

    _function->interpret();

    if( !ctx->existReturnValue() )
        ctx->setReturnValue();
    ctx->removeScope(tmp);
}

FunctionDefinition::~FunctionDefinition() {
    if( _function )
        delete _function;

    Context::getInstance()->removeScope(_runningContext);
    Context::getInstance()->removeScope(_context);
}

BaseFunction::BaseFunction() : FunctionDefinition() {}

BaseFunction::~BaseFunction() {}

void BaseFunction::load(list<ArgumentDefinition> arguments,void (*function)()) {
    Context* ctx = Context::getInstance();

    _context = ctx->newScope();

    for( auto argument : arguments )
    {
        ctx->newVariable(argument.getName(),argument.getDataTypeId());
    }

    _argumentsDefinitionList = arguments;

    _functionPointer = function;

    ctx->returnScope();
}

void BaseFunction::interpret(LiteralValue* literalArgs) {
    if( literalArgs->getDataTypeId() != DataTypesId::Argument )
        throw SyntaxException("Invalid arguments");

    list<LiteralValue*> args = *(list<LiteralValue*>*)literalArgs->getValue();

    if( args.size() != _argumentsDefinitionList.size() )
        throw SyntaxException("Invalid number of arguments");

    Context* ctx = Context::getInstance();
    _runningContext = ctx->switchScope(_context);
    scope_index tmp = _runningContext;

    for( auto argumentDefinition : _argumentsDefinitionList )
    {
        ctx->setVariableValue(argumentDefinition.getName(),args.front());
        args.pop_front();
    }

    _functionPointer();

    if( !ctx->existReturnValue() )
        ctx->setReturnValue();

    ctx->removeScope(tmp);
}
