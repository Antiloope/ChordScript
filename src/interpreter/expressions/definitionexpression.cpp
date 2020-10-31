#include "definitionexpression.h"
#include "interpreter/functiondefinition.h"
#include "terminalexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/context.h"

using namespace CS;
using namespace std;

DefinitionExpression::DefinitionExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void DefinitionExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* aux = terminalExpressionsList->front();

    Context* ctx = Context::getInstance();

    if(
        aux->getType() != cCast(TerminalExpressionType::Name) ||
        !ctx->isDataType(DataType::getDataTypeId(((NameExpression*)aux)->getName())) )
        throw SyntaxException("Expected data type",aux->getCodeReference());

    DataTypesId dataType = DataType::getDataTypeId(((NameExpression*)aux)->getName());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected a name",aux->getCodeReference() );

    delete aux;
    aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::Name) )
        throw SyntaxException("Expected a name",aux->getCodeReference());

    if( !ctx->isValidName(((NameExpression*)aux)->getName()) )
        throw SyntaxException("Invalid name", aux->getCodeReference());

    string varName = ((NameExpression*)aux)->getName();

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected ; or a function definition",aux->getCodeReference() );

    delete aux;
    aux = terminalExpressionsList->front();

    if( aux->getType() == cCast(TerminalExpressionType::EOE) )
    {
        terminalExpressionsList->pop_front();
        ctx->newVariable(varName,dataType);
    }
    else if( aux->getType() == cCast(TerminalExpressionType::OpenParenthesis) )
    {
        auto function = ctx->newFunction(varName,dataType);
        function->load(terminalExpressionsList);
    }
    else
        throw SyntaxException("Expected ; or a function definition",aux->getCodeReference());
}

DefinitionExpression::~DefinitionExpression() {}

void DefinitionExpression::interpret() {}
