#include "expression.h"

using namespace CS;

Expression::Expression() {}

Expression::~Expression() {}

void EqualExpression::interpret() {}

void EOEExpression::interpret() {}

void SubstractionExpression::interpret() {}

void AdditionExpression::interpret() {}

void MultiplicationExpression::interpret() {}

void DivitionExpression::interpret() {}

void NegationExpression::interpret() {}

void OrExpression::interpret() {}

void AndExpression::interpret() {}

void MemberAccessExpression::interpret() {}

void SeparatorExpression::interpret() {}

void OpenParenthesisExpression::interpret() {}

void CloseParenthesisExpression::interpret() {}

void OpenBracketExpression::interpret() {}

void CloseBracketExpression::interpret() {}

void OpenBraceExpression::interpret() {}

void CloseBraceExpression::interpret() {}

StringExpression::StringExpression(string text) : _text(text) {}

void StringExpression::interpret() {}

NameExpression::NameExpression(string name) : _name(name) {}

void NameExpression::interpret() {}

void ForExpression::interpret() {}

void IfExpression::interpret() {}

void BreakExpression::interpret() {}

void ReturnExpression::interpret() {}

void SampleTypeExpression::interpret() {}

void SoundTypeExpression::interpret() {}

void RealTypeExpression::interpret() {}

void IntegerTypeExpression::interpret() {}

void ArgumentTypeExpression::interpret() {}

void GroupTypeExpression::interpret() {}

void BoolTypeExpression::interpret() {}

void BufferTypeExpression::interpret() {}

void StringTypeExpression::interpret() {}

BooleanValueExpression::BooleanValueExpression(bool boolValue) :
    _boolValue(boolValue) {}

void BooleanValueExpression::interpret() {}

IntegerNumberExpression::IntegerNumberExpression(int integerNumber) :
    BooleanValueExpression((bool)integerNumber), _integerNumber(integerNumber) {}

void IntegerNumberExpression::interpret() {}

RealNumberExpression::RealNumberExpression(double realNumber) :
    IntegerNumberExpression((int)realNumber), _realNumber(realNumber) {}

void RealNumberExpression::interpret() {}
