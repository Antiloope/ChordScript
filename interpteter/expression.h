#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <queue>

using namespace std;

namespace CS {

// Implementation of intepreter pattern
class Expression {
public:
    Expression();
    virtual ~Expression();
    virtual void interpret(queue<Expression*>*);
};

class EqualExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class EOEExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class SubstractionExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class AdditionExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class MultiplicationExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class DivitionExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class NegationExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class OrExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class AndExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class MemberAccessExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class SeparatorExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class OpenParenthesisExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class CloseParenthesisExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class OpenBracketExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class CloseBracketExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class OpenBraceExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class CloseBraceExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class StringExpression : public Expression {
public:
    StringExpression(string text);
    ~StringExpression(){};
    void interpret(queue<Expression*>*) override;
protected:
    string _text;
};

class NameExpression : public Expression {
public:
    NameExpression(string name);
    void interpret(queue<Expression*>*) override;
protected:
    string _name;
};

class ForExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class IfExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class BreakExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class ReturnExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class SampleTypeExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class SoundTypeExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class RealTypeExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class IntegerTypeExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class ArgumentTypeExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class GroupTypeExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class BoolTypeExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class BufferTypeExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class StringTypeExpression : public Expression {
public:
    void interpret(queue<Expression*>*) override;
};

class BooleanValueExpression : public Expression {
public:
    BooleanValueExpression(bool boolValue);
    void interpret(queue<Expression*>*) override;
protected:
    bool _boolValue;
};

class IntegerNumberExpression : public BooleanValueExpression {
public:
    IntegerNumberExpression(int integerNumber);
    void interpret(queue<Expression*>*) override;
protected:
    int _integerNumber;
};

class RealNumberExpression : public IntegerNumberExpression {
public:
    RealNumberExpression(double realNumber);
    void interpret(queue<Expression*>*) override;
protected:
    double _realNumber;
};

}

#endif // EXPRESSION_H
