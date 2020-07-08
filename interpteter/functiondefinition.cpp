#include "functiondefinition.h"
#include "context.h"

using namespace CS;

ArgumentDefinition::ArgumentDefinition(string dataType, string name, bool byReference) : _dataType(dataType),_name(name),_byReference(byReference) {}

FunctionDefinition::FunctionDefinition(list<TerminalExpression*>* terminalEspressionsList)
{
    TerminalExpression *tmp = terminalEspressionsList->front();

    Context* ctx  =Context::getInstance();

    if( ctx->getCurrentContext() != GlobalContext ) throw SemanticException("Functions must be defined on global scope",tmp->getCodeReference());

    _context = ctx->newContext();

    try {
        if( tmp->getType() != cCast(ExpressionTypes::OpenParenthesis) ) throw SyntaxException("Expected (",tmp->getCodeReference());

        terminalEspressionsList->pop_front();
        if( terminalEspressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
        tmp = terminalEspressionsList->front();

        string dataType;
        bool byReference;

        bool isValidArgument = true;
        while( isValidArgument )
        {
            if ( tmp->getType() == cCast(ExpressionTypes::Name) )
            {
                if ( ctx->isDataType(((NameExpression*)tmp)->getName()) )
                {
                    dataType = ((NameExpression*)tmp)->getName();

                    terminalEspressionsList->pop_front();
                    if( terminalEspressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                    tmp = terminalEspressionsList->front();

                    byReference = false;
                    if ( tmp->getType() == cCast(ExpressionTypes::And) )
                    {
                        byReference = true;

                        terminalEspressionsList->pop_front();
                        if( terminalEspressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                        tmp = terminalEspressionsList->front();
                    }

                    if( tmp->getType() == cCast(ExpressionTypes::Name) )
                    {
                        if( ctx->isValidName(((NameExpression*)tmp)->getName()) )
                        {
                            ctx->newVariable(((NameExpression*)tmp)->getName(),dataType,nullptr);

                            _argumentsDefinitionList.push_back(ArgumentDefinition(dataType,((NameExpression*)tmp)->getName(),byReference));

                            terminalEspressionsList->pop_front();
                            if( terminalEspressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                            tmp = terminalEspressionsList->front();

                            if( tmp->getType() == cCast(ExpressionTypes::Separator) )
                            {
                                terminalEspressionsList->pop_front();
                                if( terminalEspressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                                tmp = terminalEspressionsList->front();

                                if( tmp->getType() == cCast(ExpressionTypes::CloseParenthesis) )
                                {
                                    isValidArgument = false;

                                    terminalEspressionsList->pop_front();
                                    if( terminalEspressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                                    tmp = terminalEspressionsList->front();
                                }
                            }
                            else if( tmp->getType() == cCast(ExpressionTypes::CloseParenthesis) )
                            {
                                isValidArgument = false;

                                terminalEspressionsList->pop_front();
                                if( terminalEspressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                                tmp = terminalEspressionsList->front();
                            }
                            else
                            {
                                throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                            }
                        }
                        else
                        {
                            throw SyntaxException("Invalid name",tmp->getCodeReference());
                        }
                    }
                    else
                    {
                        throw SyntaxException("Expected argument name",tmp->getCodeReference());
                    }
                }
                else
                {
                    throw SyntaxException("Expected data type", tmp->getCodeReference());
                }
            }
            else if( tmp->getType() == cCast(ExpressionTypes::CloseParenthesis) )
            {
                isValidArgument = false;

                terminalEspressionsList->pop_front();
                if( terminalEspressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
                tmp = terminalEspressionsList->front();
            }
            else
            {
                throw SyntaxException("Expected arguments", tmp->getCodeReference());
            }
        }

        if( tmp->getType() != cCast(ExpressionTypes::OpenBrace) ) throw SyntaxException("Expected {",tmp->getCodeReference());

        terminalEspressionsList->pop_front();
        if( terminalEspressionsList->empty() ) throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
        tmp = terminalEspressionsList->front();

        _function = new ProgramExpression(terminalEspressionsList,tmp->getCodeReference());

        tmp = terminalEspressionsList->front();
        if( terminalEspressionsList->empty() ) throw SyntaxException("Expected }",tmp->getCodeReference() );

        if ( tmp->getType() == cCast(ExpressionTypes::CloseBrace) )
        {
            terminalEspressionsList->pop_front();
        }
        else
        {
            throw SyntaxException("Expected }",tmp->getCodeReference() );
        }

        ctx->switchContext(GlobalContext);

    } catch (SyntaxException &e) {
        ctx->switchContext(GlobalContext);
        throw e;
    } catch (SemanticException &e) {
        ctx->switchContext(GlobalContext);
        throw e;
    } catch (exception &e) {
        ctx->switchContext(GlobalContext);
        throw e;
    }
}

