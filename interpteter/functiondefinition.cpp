#include "functiondefinition.h"
#include "context.h"

using namespace CS;

ArgumentDefinition::ArgumentDefinition(string dataType, string name, bool byReference) : _dataType(dataType),_name(name),_byReference(byReference) {}

FunctionDefinition::FunctionDefinition(list<TerminalExpression*>* terminalEspressionsList)
{
    TerminalExpression *tmp = terminalEspressionsList->front();
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
            if ( Context::getInstance()->isDataType(((NameExpression*)tmp)->getName()) )
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
                    if( Context::getInstance()->isValidName(((NameExpression*)tmp)->getName()) )
                    {
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
}
