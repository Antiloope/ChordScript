#include "expression.h"
#include "utils/Exceptions/exception.h"
#include "context.h"

using namespace CS;

//bool EOEExpression::isEOE(list<Expression*>::iterator it){
//    Expression *tmp = *it;
//    return typeid (*tmp).hash_code() == typeid (EOEExpression).hash_code();
//}

//bool AssignationExpression::isAssignation(list<Expression*>::iterator it){
//    Expression *tmp = *it;
//    if( typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
//        Context::getInstance()->isDataType( ((NameExpression*)tmp)->_name ))
//    {
//        tmp = *(++it);
//        if( typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
//            !Context::getInstance()->nameExist( ((NameExpression*)tmp)->_name ))
//        {
//            tmp = *(++it);
//            if( typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code() )
//            {
//                tmp = *(++it);
//                if ( ValueExpression::isValue(it) )
//                {
//                    return true;
//                }
//            }
//        }
//    }
//    else
//    {
//        if( typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
//            !Context::getInstance()->nameExist( ((NameExpression*)tmp)->_name ))
//        {
//            tmp = *(++it);
//            if( typeid (*tmp).hash_code() == typeid (EqualExpression).hash_code() )
//            {
//                tmp = *(++it);
//                if ( ValueExpression::isValue(it) )
//                {
//                    return true;
//                }
//            }
//        }
//    }
//    return false;
//}
//bool ValueExpression::isValue(list<Expression*>::iterator it)
//{
//    Expression *tmp = *it;
//    if (ExecutionExpression::isExecution(it))
//    {
//        return true;
//    }
//    if (BooleanOperationExpression::isBooleanOperation(it))
//    {
//        return true;
//    }
//    if (MathOperationExpression::isMathOperation(it))
//    {
//        return true;
//    }
//    if (typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
//        Context::getInstance()->nameExist(((NameExpression*)tmp)->_name ))
//    {
//        return true;
//    }
//    if (typeid (*tmp).hash_code() == typeid (StringExpression).hash_code())
//    {
//        return true;
//    }
//    if (typeid (*tmp).hash_code() == typeid (NullValueExpression).hash_code())
//    {
//        return true;
//    }
//    if (typeid (*tmp).hash_code() == typeid (OpenParenthesisExpression).hash_code())
//    {
//        tmp = *(++it);
//        if (ArrayExpression::isArray(it))
//        {
//            tmp = *(++it);
//            if (typeid (*tmp).hash_code() == typeid (CloseParenthesisExpression).hash_code())
//            {
//                return true;
//            }
//        }
//        return false;
//    }
//    if (typeid (*tmp).hash_code() == typeid (OpenBracketExpression).hash_code())
//    {
//        tmp = *(++it);
//        if (ArrayExpression::isArray(it))
//        {
//            tmp = *(++it);
//            if (typeid (*tmp).hash_code() == typeid (CloseBracketExpression).hash_code())
//            {
//                return true;
//            }
//        }
//        return false;
//    }
//    return false;
//}

//bool ArrayExpression::isArray(list<Expression *>::iterator it){
//    Expression *tmp = *it;
//    if(ValueExpression::isValue(it))
//    {
//        tmp = *(++it);
//        if (typeid (*tmp).hash_code() == typeid (SeparatorExpression).hash_code())
//        {
//            tmp = *(++it);
//            if(ArrayExpression::isArray(it))
//            {
//                return true;
//            }
//            return false;
//        }
//        return true;
//    }
//    return false;
//}

//bool MathOperationExpression::isMathOperation(list<Expression *>::iterator it){
//    Expression *tmp = *it;
//    if( typeid (*tmp).hash_code() == typeid (AdditionExpression).hash_code() ||
//        typeid (*tmp).hash_code() == typeid (SubstractionExpression).hash_code())
//    {
//        tmp = *(++it);
//    }
//    if( typeid (*tmp).hash_code() == typeid (OpenParenthesisExpression).hash_code())
//    {
//        tmp = *(++it);
//        if(MathOperationExpression::isMathOperation(it))
//        {
//            tmp = *(++it);
//            if(typeid (*tmp).hash_code() == typeid (CloseParenthesisExpression).hash_code())
//            {
//                return true;
//            }
//            return false;
//        }
//        return false;
//    }
//    if( typeid (*tmp).hash_code() == typeid (IntegerNumberExpression).hash_code() ||
//        typeid (*tmp).hash_code() == typeid (RealNumberExpression).hash_code() ||
//       (typeid (*tmp).hash_code() == typeid (NameExpression).hash_code() &&
//       !Context::getInstance()->nameExist(((NameExpression*)tmp)->_name)) ||
//        ExecutionExpression::isExecution(it))
//    {
//        tmp = *(++it);
//        if(MathOperatorExpression::isMathOperator(it))
//        {
//            tmp = *(++it);
//            if(MathOperationExpression::isMathOperation(it))
//            {
//                return true;
//            }
//            return false;
//        }
//        return true;
//    }
//    return false;
//}

//bool MathOperatorExpression::isMathOperator(list<Expression *>::iterator it){
//    Expression *tmp = *it;
//    if( typeid (*tmp).hash_code() == typeid (AdditionExpression).hash_code() ||
//        typeid (*tmp).hash_code() == typeid (SubstractionExpression).hash_code() ||
//        typeid (*tmp).hash_code() == typeid (MultiplicationExpression).hash_code() ||
//        typeid (*tmp).hash_code() == typeid (DivitionExpression).hash_code())
//    {
//        return true;
//    }
//    return false;
//}

//bool BooleanOperationExpression::isBooleanOperation(list<Expression *>::iterator it){
//    Expression *tmp = *it;
//    if( typeid (*tmp).hash_code() == typeid (NegationExpression).hash_code())
//    {
//        tmp = *(++it);
//    }
//    if( typeid (*tmp).hash_code() == typeid (OpenParenthesisExpression).hash_code())
//    {
//        tmp = *(++it);
//        if(BooleanOperationExpression::isBooleanOperation(it))
//        {
//            tmp = *(++it);
//            if(typeid (*tmp).hash_code() == typeid (CloseParenthesisExpression).hash_code())
//            {
//                return true;
//            }
//            return false;
//        }
//        return false;
//    }
//    if( ValueExpression::isValue(it) ||
//        typeid (*tmp).hash_code() == typeid (BooleanValueExpression).hash_code()
//        )
//    {
//        tmp = *(++it);
//        if(BooleanOperatorExpression::isBooleanOperator(it))
//        {
//            tmp = *(++it);
//            if(BooleanOperationExpression::isBooleanOperation(it))
//            {
//                return true;
//            }
//            return false;
//        }
//        return true;
//    }
//    return false;
//}

//bool BooleanOperatorExpression::isBooleanOperator(list<Expression *>::iterator it){
//    Expression *tmp = *it;
//    if( typeid (*tmp).hash_code() == typeid (GreaterThanExpression).hash_code() ||
//        typeid (*tmp).hash_code() == typeid (LessThanExpression).hash_code())
//    {

//        return true;
//    }
//    return false;
//}

//bool ExecutionExpression::isExecution(list<Expression*>::iterator it){
//    return true;
//}

//bool DefinitionExpression::isDefinition(list<Expression*>::iterator it){
//    return true;
//}

//bool NullValueExpression::isNull(list<Expression *>::iterator it){
//    return true;
//}

//bool ForExpression::isFor(list<Expression*>::iterator it){
//    return true;
//}

//bool IfExpression::isIf(list<Expression*>::iterator it){
//    return true;
//}

//bool BreakExpression::isBreak(list<Expression*>::iterator it){
//    return true;
//}

//Expression::Expression(size_t codeReference):_codeReference(codeReference) {}

//Expression::~Expression() {}

//size_t Expression::getCodeReference(){
//    return _codeReference;
//}

//void Expression::interpret(list<Expression*>* expressionsList){
//    while(!expressionsList->empty()){
//        auto it = expressionsList->begin();
//        Expression* tmp = *it;
//        try {
//            if( EOEExpression::isEOE( it ) ) {
//                tmp = new EOEExpression(tmp->getCodeReference());
//            }
//            else if( AssignationExpression::isAssignation( it ) ){
//                tmp = new AssignationExpression(tmp->getCodeReference());
//            }
//            else if( ExecutionExpression::isExecution( it ) ){
//                tmp = new ExecutionExpression(tmp->getCodeReference());
//            }
//            else if( DefinitionExpression::isDefinition( it ) ){
//                tmp = new DefinitionExpression(tmp->getCodeReference());
//            }
//            else if( ForExpression::isFor( it ) ){
//                tmp = new ForExpression(tmp->getCodeReference());
//            }
//            else if( IfExpression::isIf( it ) ){
//                tmp = new IfExpression(tmp->getCodeReference());
//            }
//            else if( BreakExpression::isBreak( it ) ){
//                tmp = new BreakExpression(tmp->getCodeReference());
//            }
//            else {
//                throw SyntaxException("Syntax error",tmp->getCodeReference());
//            }
//        } catch (SyntaxException& e) {
//            throw e;
//        } catch (exception& e){
//            throw e;
//        }

//        tmp->interpret(expressionsList);

//        delete tmp;
//    }
//}
Expression::Expression() : _codeReference(0) {

}
Expression::Expression(size_t codeReference) : _codeReference(codeReference) {

}
Expression::~Expression(){

}
size_t Expression::getCodeReference() {
    return _codeReference;
}
