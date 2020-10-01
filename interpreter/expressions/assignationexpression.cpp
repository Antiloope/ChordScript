#include "assignationexpression.h"
#include "utils/Exceptions/exception.h"
#include "interpteter/context.h"
#include "terminalexpression.h"
#include "interpteter/values/linkedvalue.h"

using namespace CS;
using namespace std;

AssignationExpression::AssignationExpression(size_t codeReference) :
    NonTerminalExpression(codeReference) {}

void AssignationExpression::load(list<TerminalExpression*>* terminalExpressionsList) {
    TerminalExpression* tmp = terminalExpressionsList->front();
    DataTypesId dataType;
    bool isDataTypeDefinition = false;

    if(
        tmp->getType() == cCast(TerminalExpressionType::Name) &&
        Context::getInstance()->isDataType(DataType::getDataTypeId(((NameExpression*)tmp)->getName())) )
    {
        dataType = DataType::getDataTypeId(((NameExpression*)tmp)->getName());

        terminalExpressionsList->pop_front();
        if( terminalExpressionsList->empty())
            throw SyntaxException("Expected variable name",tmp->getCodeReference() );
        tmp = terminalExpressionsList->front();

        if( tmp->getType() != cCast(TerminalExpressionType::Name) )
            throw SyntaxException("Expected variable name",tmp->getCodeReference());
        isDataTypeDefinition = true;
    }
    else if( Context::getInstance()->nameExist(((NameExpression*)tmp)->getName()) )
        dataType = Context::getInstance()->getVariableDataType(((NameExpression*)tmp)->getName());
    else
        throw SyntaxException("Expected another symbol",tmp->getCodeReference());

    _varName = ((NameExpression*)tmp)->getName();

    if( !Context::getInstance()->isValidName(_varName) )
        throw SyntaxException("Invalid name",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected = after name",tmp->getCodeReference() );
    tmp = terminalExpressionsList->front();

    if( tmp->getType() != cCast(TerminalExpressionType::Equal) )
        throw SyntaxException("Expected = ",tmp->getCodeReference());

    terminalExpressionsList->pop_front();
    if( terminalExpressionsList->empty())
        throw SyntaxException("Expected a value after = ",tmp->getCodeReference() );
    terminalExpressionsList->front();

    _value = LinkedValue::generateLinkedValue(terminalExpressionsList);
    if (
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::EOE) &&
        terminalExpressionsList->front()->getType() != cCast(TerminalExpressionType::CloseParenthesis)
        )
        throw SyntaxException("Expected ;",terminalExpressionsList->front()->getCodeReference());

    if( isDataTypeDefinition )
        Context::getInstance()->newVariable( _varName, dataType);
}

AssignationExpression::~AssignationExpression() {
    if( _value )
        delete _value;
}

void AssignationExpression::interpret() {
    if( !Context::getInstance()->setVariableValue(_varName,_value->getValue()) )
    {
        string errorDescription = "Invalid conversion from ";
        errorDescription.append(DataType::getDataTypeString(_value->getDataTypeId()));
        errorDescription.append(" to ");
        errorDescription.append(
            DataType::getDataTypeString(
                Context::getInstance()->getVariableDataType(_varName))
            );
        throw SemanticException(errorDescription,this->getCodeReference());
    }
    Context::getInstance()->setReturnValue(nullptr);
}
