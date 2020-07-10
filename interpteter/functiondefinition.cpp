#include "functiondefinition.h"
#include "context.h"

using namespace CS;

ArgumentDefinition::ArgumentDefinition(string dataType, string name, bool byReference) : _dataType(dataType),_name(name),_byReference(byReference) {}

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

    string dataType;
    bool byReference;

    bool isValidArgument = true;
    while( isValidArgument )
    {
        if ( tmp->getType() == cCast(ExpressionTypes::Name) )
        {
            if ( !ctx->isDataType(((NameExpression*)tmp)->getName()) ) throw SyntaxException("Expected data type", tmp->getCodeReference());

            dataType = ((NameExpression*)tmp)->getName();

            terminalExpressionsList->pop_front();
            if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
            tmp = terminalExpressionsList->front();

            byReference = false;
            if ( tmp->getType() == cCast(ExpressionTypes::And) )
            {
                byReference = true;

                terminalExpressionsList->pop_front();
                if( terminalExpressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                tmp = terminalExpressionsList->front();
            }

            if( tmp->getType() != cCast(ExpressionTypes::Name) ) throw SyntaxException("Expected argument name",tmp->getCodeReference());

            if( !ctx->isValidName(((NameExpression*)tmp)->getName()) ) throw SyntaxException("Invalid name",tmp->getCodeReference());

            ctx->newVariable(((NameExpression*)tmp)->getName(),dataType,nullptr);

            _argumentsDefinitionList.push_back(ArgumentDefinition(dataType,((NameExpression*)tmp)->getName(),byReference));

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

    ctx->switchContext(GlobalContext);
}

void FunctionDefinition::interpret(LiteralValue*) {

}

FunctionDefinition::~FunctionDefinition() {
    if( _function ) delete _function;

    Context::getInstance()->removeContext(_context);
}
