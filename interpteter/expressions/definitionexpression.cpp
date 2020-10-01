#include "definitionexpression.h"
#include "interpteter/functiondefinition.h"
#include "terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpteter/context.h"

using namespace CS;
using namespace std;

DefinitionExpression::DefinitionExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void DefinitionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();

    if(
        tmp->getType() != cCast(TerminalExpressionType::Name) ||
        !Context::getInstance()->isDataType(DataType::getDataTypeId(((NameExpression*)tmp)->getName()))
        )
        throw SyntaxException("Expected data type",tmp->getCodeReference());

    DataTypesId dataType = DataType::getDataTypeId(((NameExpression*)tmp)->getName());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected a name",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    if( tmp->getType() != cCast(TerminalExpressionType::Name) )
        throw SyntaxException("Expected a name",tmp->getCodeReference());

    if( !Context::getInstance()->isValidName(((NameExpression*)tmp)->getName()) )
        throw SyntaxException("Invalid name", tmp->getCodeReference());

    string varName = ((NameExpression*)tmp)->getName();

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected another symbol",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    if( tmp->getType() == cCast(TerminalExpressionType::EOE) )
    {
        terminalExpressionsList->pop_front();
        Context::getInstance()->newVariable(varName,dataType);
    }
    else if ( tmp->getType() == cCast(TerminalExpressionType::OpenParenthesis) )
    {
        FunctionDefinition* function = Context::getInstance()->newFunction(varName,dataType);
        function->load(terminalExpressionsList);
    }
    else
        throw SyntaxException("Expected an end of line or a function definition",tmp->getCodeReference());
}

DefinitionExpression::~DefinitionExpression() {}

void DefinitionExpression::interpret() {}
