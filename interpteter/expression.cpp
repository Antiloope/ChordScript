#include "expression.h"
#include "utils/Exceptions/exception.h"

using namespace CS;

bool isEOE(Expression* a){
    return typeid (*a).hash_code() == typeid (EOEExpression).hash_code();
}

bool isASSIGNATION(Expression* a){
    return typeid (*a).hash_code() == typeid (NameExpression).hash_code();
}

bool isEXECUTION(Expression* a){
    return typeid (*a).hash_code() == typeid (NameExpression).hash_code();
}

bool isDEFINITION(Expression* a){
    return typeid (*a).hash_code() == typeid (NameExpression).hash_code();
}

bool isFOR_LOOP(Expression* a){
    return typeid (*a).hash_code() == typeid (ForExpression).hash_code();
}

bool isIF(Expression* a){
    return typeid (*a).hash_code() == typeid (IfExpression).hash_code();
}

bool isBREAK(Expression* a){
    return typeid (*a).hash_code() == typeid (BreakExpression).hash_code();
}

Expression::Expression() {}

Expression::~Expression() {}

void Expression::interpret(queue<Expression*>* q){
    while(!q->empty()){
        Expression* tmp = q->front();
        q->pop();

        if( isEOE(tmp) ) {} // Next expression
        else if(
                isASSIGNATION(tmp) ||
                isEXECUTION(tmp) ||
                isDEFINITION(tmp) ||
                isFOR_LOOP(tmp) ||
                isIF(tmp) ||
                isBREAK(tmp)
                ){
            tmp->interpret(q);
        }
        else throw SyntaxException("Syntax error");
    }
}

void EqualExpression::interpret(queue<Expression*>* q) {}

void EOEExpression::interpret(queue<Expression*>* q) {}

void SubstractionExpression::interpret(queue<Expression*>* q) {}

void AdditionExpression::interpret(queue<Expression*>* q) {}

void MultiplicationExpression::interpret(queue<Expression*>* q) {}

void DivitionExpression::interpret(queue<Expression*>* q) {}

void NegationExpression::interpret(queue<Expression*>* q) {}

void OrExpression::interpret(queue<Expression*>* q) {}

void AndExpression::interpret(queue<Expression*>* q) {}

void MemberAccessExpression::interpret(queue<Expression*>* q) {}

void SeparatorExpression::interpret(queue<Expression*>* q) {}

void OpenParenthesisExpression::interpret(queue<Expression*>* q) {}

void CloseParenthesisExpression::interpret(queue<Expression*>* q) {}

void OpenBracketExpression::interpret(queue<Expression*>* q) {}

void CloseBracketExpression::interpret(queue<Expression*>* q) {}

void OpenBraceExpression::interpret(queue<Expression*>* q) {}

void CloseBraceExpression::interpret(queue<Expression*>* q) {}

StringExpression::StringExpression(string text) : _text(text) {}

void StringExpression::interpret(queue<Expression*>* q) {}

NameExpression::NameExpression(string name) : _name(name) {}

void NameExpression::interpret(queue<Expression*>* q) {}

void ForExpression::interpret(queue<Expression*>* q) {}

void IfExpression::interpret(queue<Expression*>* q) {}

void BreakExpression::interpret(queue<Expression*>* q) {}

void ReturnExpression::interpret(queue<Expression*>* q) {}

void SampleTypeExpression::interpret(queue<Expression*>* q) {}

void SoundTypeExpression::interpret(queue<Expression*>* q) {}

void RealTypeExpression::interpret(queue<Expression*>* q) {}

void IntegerTypeExpression::interpret(queue<Expression*>* q) {}

void ArgumentTypeExpression::interpret(queue<Expression*>* q) {}

void GroupTypeExpression::interpret(queue<Expression*>* q) {}

void BoolTypeExpression::interpret(queue<Expression*>* q) {}

void BufferTypeExpression::interpret(queue<Expression*>* q) {}

void StringTypeExpression::interpret(queue<Expression*>* q) {}

BooleanValueExpression::BooleanValueExpression(bool boolValue) :
    _boolValue(boolValue) {}

void BooleanValueExpression::interpret(queue<Expression*>* q) {}

IntegerNumberExpression::IntegerNumberExpression(int integerNumber) :
    BooleanValueExpression((bool)integerNumber), _integerNumber(integerNumber) {}

void IntegerNumberExpression::interpret(queue<Expression*>* q) {}

RealNumberExpression::RealNumberExpression(double realNumber) :
    IntegerNumberExpression((int)realNumber), _realNumber(realNumber) {}

void RealNumberExpression::interpret(queue<Expression*>* q) {}
