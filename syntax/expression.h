#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

using namespace std;

namespace CS {

// Implementation of intepreter pattern
class Expression {
public:
    Expression();
    virtual ~Expression();
    virtual void interpret() = 0;
};

class EqualExpression : public Expression {
public:
    void interpret() override;
};

class EOEExpression : public Expression {
public:
    void interpret() override;
};

class SubstractionExpression : public Expression {
public:
    void interpret() override;
};

class AdditionExpression : public Expression {
public:
    void interpret() override;
};

class MultiplicationExpression : public Expression {
public:
    void interpret() override;
};

class DivitionExpression : public Expression {
public:
    void interpret() override;
};

class NegationExpression : public Expression {
public:
    void interpret() override;
};

class OrExpression : public Expression {
public:
    void interpret() override;
};

class AndExpression : public Expression {
public:
    void interpret() override;
};

class MemberAccessExpression : public Expression {
public:
    void interpret() override;
};

class SeparatorExpression : public Expression {
public:
    void interpret() override;
};

class OpenParenthesisExpression : public Expression {
public:
    void interpret() override;
};

class CloseParenthesisExpression : public Expression {
public:
    void interpret() override;
};

class OpenBracketExpression : public Expression {
public:
    void interpret() override;
};

class CloseBracketExpression : public Expression {
public:
    void interpret() override;
};

class OpenBraceExpression : public Expression {
public:
    void interpret() override;
};

class CloseBraceExpression : public Expression {
public:
    void interpret() override;
};

class StringExpression : public Expression {
public:
    StringExpression(string text);
    ~StringExpression(){};
    void interpret() override;
protected:
    string _text;
};

class NameExpression : public Expression {
public:
    NameExpression(string name);
    void interpret() override;
protected:
    string _name;
};

class ForExpression : public Expression {
public:
    void interpret() override;
};

class IfExpression : public Expression {
public:
    void interpret() override;
};

class BreakExpression : public Expression {
public:
    void interpret() override;
};

class ReturnExpression : public Expression {
public:
    void interpret() override;
};

class SampleTypeExpression : public Expression {
public:
    void interpret() override;
};

class SoundTypeExpression : public Expression {
public:
    void interpret() override;
};

class RealTypeExpression : public Expression {
public:
    void interpret() override;
};

class IntegerTypeExpression : public Expression {
public:
    void interpret() override;
};

class ArgumentTypeExpression : public Expression {
public:
    void interpret() override;
};

class GroupTypeExpression : public Expression {
public:
    void interpret() override;
};

class BoolTypeExpression : public Expression {
public:
    void interpret() override;
};

class BufferTypeExpression : public Expression {
public:
    void interpret() override;
};

class StringTypeExpression : public Expression {
public:
    void interpret() override;
};

class BooleanValueExpression : public Expression {
public:
    BooleanValueExpression(bool boolValue);
    void interpret() override;
protected:
    bool _boolValue;
};

class IntegerNumberExpression : public BooleanValueExpression {
public:
    IntegerNumberExpression(int integerNumber);
    void interpret() override;
protected:
    int _integerNumber;
};

class RealNumberExpression : public IntegerNumberExpression {
public:
    RealNumberExpression(double realNumber);
    void interpret() override;
protected:
    double _realNumber;
};

}

#endif // EXPRESSION_H
