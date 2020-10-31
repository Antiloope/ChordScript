#include "assignationexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpreter/context.h"
#include "terminalexpression.h"
#include "interpreter/values/linkedvalue.h"

using namespace CS;
using namespace std;

AssignationExpression::AssignationExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void AssignationExpression::load(
    list<TerminalExpression*>* terminalExpressionsList) {

    TerminalExpression* aux = terminalExpressionsList->front();

    DataTypesId dataType;
    bool isDataTypeDefinition = false;

    Context* ctx = Context::getInstance();

    if(
        aux->getType() == cCast(TerminalExpressionType::Name) &&
        ctx->isDataType(DataType::getDataTypeId(((NameExpression*)aux)->getName())) )
    {
        dataType = DataType::getDataTypeId(((NameExpression*)aux)->getName());

        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty())
            throw SyntaxException("Expected variable name",aux->getCodeReference() );

        delete aux;
        aux = terminalExpressionsList->front();

        if( aux->getType() != cCast(TerminalExpressionType::Name) )
            throw SyntaxException("Expected variable name",aux->getCodeReference());

        isDataTypeDefinition = true;
    }
    else if(
        aux->getType() == cCast(TerminalExpressionType::Name) &&
        ctx->nameExist(((NameExpression*)aux)->getName()) )
        dataType = ctx->getVariableDataType(((NameExpression*)aux)->getName());
    else
        throw SyntaxException("Expected an assignation",aux->getCodeReference());

    _varName = ((NameExpression*)aux)->getName();

    if( !ctx->isValidName(_varName) )
        throw SyntaxException("Invalid variable name",aux->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected = after name",aux->getCodeReference() );

    delete aux;
    aux = terminalExpressionsList->front();

    if( aux->getType() == cCast(TerminalExpressionType::Addition) )
    {
        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty())
            throw SyntaxException("Expected " + _varName + "++ ",aux->getCodeReference() );

        delete aux;
        aux = terminalExpressionsList->front();

        if( aux->getType() != cCast(TerminalExpressionType::Addition) )
            throw SyntaxException("Expected " + _varName + "++ ",aux->getCodeReference() );

        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty() ||
            (terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE) &&
             terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::CloseParenthesis)))
            throw SyntaxException("Expected ;",aux->getCodeReference() );

        terminalExpressionsList->push_front(new NumericExpression(aux->getCodeReference()-1,cCast(TerminalExpressionType::Numeric),1));
        terminalExpressionsList->push_front(new TerminalExpression(aux->getCodeReference()-2,cCast(TerminalExpressionType::Addition)));
        terminalExpressionsList->push_front(new NameExpression(aux->getCodeReference()-3,cCast(TerminalExpressionType::Name),_varName));
        terminalExpressionsList->push_front(new TerminalExpression(aux->getCodeReference()-2,cCast(TerminalExpressionType::Equal)));

        delete aux;
    }
    else if( aux->getType() == cCast(TerminalExpressionType::Substract) )
    {
        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty())
            throw SyntaxException("Expected " + _varName + "-- ",aux->getCodeReference() );

        delete aux;
        aux = terminalExpressionsList->front();

        if( aux->getType() != cCast(TerminalExpressionType::Substract) )
            throw SyntaxException("Expected " + _varName + "-- ",aux->getCodeReference() );

        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty() ||
           (terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE) &&
            terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::CloseParenthesis) ))
            throw SyntaxException("Expected ;",aux->getCodeReference() );

        terminalExpressionsList->push_front(new NumericExpression(aux->getCodeReference()-1,cCast(TerminalExpressionType::Numeric),1));
        terminalExpressionsList->push_front(new TerminalExpression(aux->getCodeReference()-2,cCast(TerminalExpressionType::Substract)));
        terminalExpressionsList->push_front(new NameExpression(aux->getCodeReference()-3,cCast(TerminalExpressionType::Name),_varName));
        terminalExpressionsList->push_front(new TerminalExpression(aux->getCodeReference()-2,cCast(TerminalExpressionType::Equal)));

        delete aux;
    }
    aux = terminalExpressionsList->front();

    if( aux->getType() != cCast(TerminalExpressionType::Equal) )
        throw SyntaxException("Expected = ",aux->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected a value after = ",aux->getCodeReference() );

    delete aux;
    aux = terminalExpressionsList->front();

    _value = LinkedValue::generateLinkedValue(terminalExpressionsList);
    if (
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE) &&
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::CloseParenthesis) )
        throw SyntaxException("Expected ;",aux->getCodeReference());

    if( isDataTypeDefinition )
        ctx->newVariable( _varName, dataType);
}

AssignationExpression::~AssignationExpression() {
    if( _value )
        delete _value;
}

void AssignationExpression::interpret() {
    Context* ctx = Context::getInstance();
    if( !ctx->setVariableValue(_varName,_value->getValue()) )
    {
        string errorDescription = "Invalid conversion from ";
        errorDescription.append(DataType::getDataTypeString(_value->getDataTypeId()));
        errorDescription.append(" to ");
        errorDescription.append(
            DataType::getDataTypeString(
                ctx->getVariableDataType(_varName)) );
        throw SemanticException(errorDescription,this->getCodeReference());
    }
    ctx->setReturnValue(nullptr);
}
