#include "expression.h"
#include "utils/Exceptions/exception.h"
#include "context.h"

using namespace CS;

bool EOEExpression::isEOE(list<Expression*>::iterator it){
    Expression *tmp = *it;
    return typeid (*tmp).hash_code() == typeid (EOEExpression).hash_code();
}

bool AssignationExpression::isAssignation(list<Expression*>::iterator it){
    Expression *tmp = *it;
    if( typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
        Context::getInstance()->isDataType( ((NameExpression*)tmp)->_name ))
    {
        tmp = *(++it);
        if( typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
            !Context::getInstance()->nameExist( ((NameExpression*)tmp)->_name ))
        {
            tmp = *(++it);
            if( typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code() )
            {
                tmp = *(++it);
                if ( ValueExpression::isValue(it) )
                {
                    return true;
                }
            }
        }
    }
    else
    {
        if( typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
            !Context::getInstance()->nameExist( ((NameExpression*)tmp)->_name ))
        {
            tmp = *(++it);
            if( typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code() )
            {
                tmp = *(++it);
                if ( ValueExpression::isValue(it) )
                {
                    return true;
                }
            }
        }
    }
    return false;
}
bool ValueExpression::isValue(list<Expression*>::iterator it)
{
    Expression *tmp = *it;
    if (typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
        Context::getInstance()->nameExist(((NameExpression*)tmp)->_name ))
    {
        return true;
    }
    if (typeid (*tmp).hash_code() == typeid (StringExpression).hash_code())
    {
        return true;
    }
    if (ExecutionExpression::isExecution(it))
    {
        return true;
    }
    if (typeid (*tmp).hash_code() == typeid (NullValueExpression).hash_code())
    {
        return true;
    }
    if (BooleanOperationExpression::isBooleanOperation(it))
    {
        return true;
    }
    if (MathOperationExpression::isMathOperation(it))
    {
        return true;
    }
    if (typeid (*tmp).hash_code() == typeid (OpenParenthesisExpression).hash_code())
    {
        tmp = *(++it);
        if (ArrayExpression::isArray(it))
        {
            tmp = *(++it);
            if (typeid (*tmp).hash_code() == typeid (CloseParenthesisExpression).hash_code())
            {
                return true;
            }
        }
        return false;
    }
    if (typeid (*tmp).hash_code() == typeid (OpenBracketExpression).hash_code())
    {
        tmp = *(++it);
        if (ArrayExpression::isArray(it))
        {
            tmp = *(++it);
            if (typeid (*tmp).hash_code() == typeid (CloseBracketExpression).hash_code())
            {
                return true;
            }
        }
        return false;
    }
    return false;
}

bool ArrayExpression::isArray(list<Expression *>::iterator it){
    Expression *tmp = *it;
    if(ValueExpression::isValue(it))
    {
        tmp = *(++it);
        if (typeid (*tmp).hash_code() == typeid (SeparatorExpression).hash_code())
        {
            tmp = *(++it);
            if(ArrayExpression::isArray(it))
            {
                return true;
            }
            return false;
        }
        return true;
    }
    return false;
}

bool MathOperationExpression::isMathOperation(list<Expression *>::iterator it){
    return true;
}

bool BooleanOperationExpression::isBooleanOperation(list<Expression *>::iterator it){
    return true;
}

bool ExecutionExpression::isExecution(list<Expression*>::iterator it){
    return true;
}

bool DefinitionExpression::isDefinition(list<Expression*>::iterator it){
    return true;
}

bool NullValueExpression::isNull(list<Expression *>::iterator it){
    return true;
}

bool ForExpression::isFor(list<Expression*>::iterator it){
    return true;
}

bool IfExpression::isIf(list<Expression*>::iterator it){
    return true;
}

bool BreakExpression::isBreak(list<Expression*>::iterator it){
    return true;
}

Expression::Expression(size_t codeReference):_codeReference(codeReference) {}

Expression::~Expression() {}

size_t Expression::getCodeReference(){
    return _codeReference;
}

void Expression::interpret(list<Expression*>* expressionsList){
    while(!expressionsList->empty()){
        auto it = expressionsList->begin();
        Expression* tmp = *it;
        try {
            if( EOEExpression::isEOE( it ) ) {
                tmp = new EOEExpression(tmp->getCodeReference());
            }
            else if( AssignationExpression::isAssignation( it ) ){
                tmp = new AssignationExpression(tmp->getCodeReference());
            }
            else if( ExecutionExpression::isExecution( it ) ){
                tmp = new ExecutionExpression(tmp->getCodeReference());
            }
            else if( DefinitionExpression::isDefinition( it ) ){
                tmp = new DefinitionExpression(tmp->getCodeReference());
            }
            else if( ForExpression::isFor( it ) ){
                tmp = new ForExpression(tmp->getCodeReference());
            }
            else if( IfExpression::isIf( it ) ){
                tmp = new IfExpression(tmp->getCodeReference());
            }
            else if( BreakExpression::isBreak( it ) ){
                tmp = new BreakExpression(tmp->getCodeReference());
            }
            else {
                throw SyntaxException("Syntax error",tmp->getCodeReference());
            }
        } catch (SyntaxException& e) {
            throw e;
        } catch (exception& e){
            throw e;
        }

        tmp->interpret(expressionsList);

        delete tmp;
    }
}

void ValueExpression::interpret(list<Expression*>* q) {}

void AssignationExpression::interpret(list<Expression*>* q) {}

void DefinitionExpression::interpret(list<Expression*>* q) {}

void ArrayExpression::interpret(list<Expression*>* q) {}

void MathOperationExpression::interpret(list<Expression*>* q) {}

void BooleanOperationExpression::interpret(list<Expression*>* q) {}

void ExecutionExpression::interpret(list<Expression*>* q) {}

void EqualExpression::interpret(list<Expression*>* q) {}

void EOEExpression::interpret(list<Expression*>* expressionsList) {
    Expression* tmp = expressionsList->front();
    expressionsList->pop_front();
    delete tmp;
}

void SubstractionExpression::interpret(list<Expression*>* q) {}

void AdditionExpression::interpret(list<Expression*>* q) {}

void MultiplicationExpression::interpret(list<Expression*>* q) {}

void DivitionExpression::interpret(list<Expression*>* q) {}

void NegationExpression::interpret(list<Expression*>* q) {}

void OrExpression::interpret(list<Expression*>* q) {}

void AndExpression::interpret(list<Expression*>* q) {}

void MemberAccessExpression::interpret(list<Expression*>* q) {}

void SeparatorExpression::interpret(list<Expression*>* q) {}

void OpenParenthesisExpression::interpret(list<Expression*>* q) {}

void CloseParenthesisExpression::interpret(list<Expression*>* q) {}

void OpenBracketExpression::interpret(list<Expression*>* q) {}

void CloseBracketExpression::interpret(list<Expression*>* q) {}

void OpenBraceExpression::interpret(list<Expression*>* q) {}

void CloseBraceExpression::interpret(list<Expression*>* q) {}

void StringExpression::interpret(list<Expression*>* q) {}

void NameExpression::interpret(list<Expression*>* q) {

}

void NullValueExpression::interpret(list<Expression *> *q ){}

void ForExpression::interpret(list<Expression*>* q) {}

void IfExpression::interpret(list<Expression*>* q) {}

void BreakExpression::interpret(list<Expression*>* q) {}

void ReturnExpression::interpret(list<Expression*>* q) {}

void SampleTypeExpression::interpret(list<Expression*>* q) {}

void SoundTypeExpression::interpret(list<Expression*>* q) {}

void RealTypeExpression::interpret(list<Expression*>* q) {}

void IntegerTypeExpression::interpret(list<Expression*>* q) {}

void ArgumentTypeExpression::interpret(list<Expression*>* q) {}

void GroupTypeExpression::interpret(list<Expression*>* q) {}

void BoolTypeExpression::interpret(list<Expression*>* q) {}

void BufferTypeExpression::interpret(list<Expression*>* q) {}

void StringTypeExpression::interpret(list<Expression*>* q) {}

void BooleanValueExpression::interpret(list<Expression*>* q) {}

void IntegerNumberExpression::interpret(list<Expression*>* q) {}

void RealNumberExpression::interpret(list<Expression*>* q) {}
