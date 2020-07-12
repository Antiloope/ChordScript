#include "functiondefinition.h"
#include "context.h"

using namespace CS;

ArgumentDefinition::ArgumentDefinition(DataTypesId dataType, string name) : _dataType(dataType),_name(name) {}

FunctionDefinition::FunctionDefinition() {}

void FunctionDefinition::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression *tmp = terminalExpressionsList->front();

    Context* ctx = Context::getInstance();

    if( ctx->getCurrentContext() != GlobalContext ) throw SemanticException("Functions must be defined on global scope",tmp->getCodeReference());

    _context = ctx->newContext();

    if( tmp->getType() != cCast(ExpressionTypes::OpenParenthesis) ) throw SyntaxException("Expected (",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    DataTypesId dataType;

    bool isValidArgument = true;
    while( isValidArgument )
    {
        if ( tmp->getType() == cCast(ExpressionTypes::Name) )
        {
            if ( !ctx->isDataType(DataType::getDataTypeId(((NameExpression*)tmp)->getName())) ) throw SyntaxException("Expected data type", tmp->getCodeReference());

            dataType = DataType::getDataTypeId(((NameExpression*)tmp)->getName());

            terminalExpressionsList->pop_front();
            if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
            tmp = terminalExpressionsList->front();

            if( tmp->getType() != cCast(ExpressionTypes::Name) ) throw SyntaxException("Expected argument name",tmp->getCodeReference());

            if( !ctx->isValidName(((NameExpression*)tmp)->getName()) ) throw SyntaxException("Invalid name",tmp->getCodeReference());

            ctx->newVariable(((NameExpression*)tmp)->getName(),dataType);

            _argumentsDefinitionList.push_back(ArgumentDefinition(dataType,((NameExpression*)tmp)->getName()));

            terminalExpressionsList->pop_front();
            if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
            tmp = terminalExpressionsList->front();

            if( tmp->getType() == cCast(ExpressionTypes::Separator) )
            {
                terminalExpressionsList->pop_front();
                if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                tmp = terminalExpressionsList->front();

                if( tmp->getType() == cCast(ExpressionTypes::CloseParenthesis) )
                {
                    isValidArgument = false;

                    terminalExpressionsList->pop_front();
                    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                    tmp = terminalExpressionsList->front();
                }
            }
            else if( tmp->getType() == cCast(ExpressionTypes::CloseParenthesis) )
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
        else if( tmp->getType() == cCast(ExpressionTypes::CloseParenthesis) )
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

    if( tmp->getType() != cCast(ExpressionTypes::OpenBrace) ) throw SyntaxException("Expected {",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    _function = new ProgramExpression(tmp->getCodeReference());
    _function->load(terminalExpressionsList);

    if( terminalExpressionsList->empty() ) throw SyntaxException("Expected }",tmp->getCodeReference() );

    tmp = terminalExpressionsList->front();
    if ( tmp->getType() != cCast(ExpressionTypes::CloseBrace) ) throw SyntaxException("Expected }",tmp->getCodeReference() );

    terminalExpressionsList->pop_front();

    ctx->returnContext();
}

void FunctionDefinition::interpret(LiteralValue*) {

}

FunctionDefinition::~FunctionDefinition() {
    if( _function ) delete _function;

    Context::getInstance()->removeContext(_context);
}
