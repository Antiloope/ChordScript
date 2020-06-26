#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <list>

using namespace std;

namespace CS {

// Implementation of intepreter pattern
class Expression {
public:
    Expression(size_t codeReference);
    virtual ~Expression();
    virtual void interpret(list<Expression*>*);
    size_t getCodeReference();
private:
    size_t _codeReference;
};

class ValueExpression : public Expression {
public:
    ValueExpression(size_t codeReference):Expression(codeReference){}
    static bool isValue(list<Expression*>::iterator it);
    void interpret(list<Expression*>*) override;
};

class AssignationExpression : public Expression {
public:
    AssignationExpression(size_t codeReference):Expression(codeReference){}
    static bool isAssignation(list<Expression*>::iterator it);
    void interpret(list<Expression*>*) override;
};

class ArrayExpression : public Expression {
public:
    ArrayExpression(size_t codeReference):Expression(codeReference){}
    static bool isArray(list<Expression*>::iterator it);
    void interpret(list<Expression*>*) override;
};

class MathOperationExpression : public Expression {
public:
    MathOperationExpression(size_t codeReference):Expression(codeReference){}
    static bool isMathOperation(list<Expression*>::iterator it);
    void interpret(list<Expression*>*) override;
};

class BooleanOperationExpression : public Expression {
public:
    BooleanOperationExpression(size_t codeReference):Expression(codeReference){}
    static bool isBooleanOperation(list<Expression*>::iterator it);
    void interpret(list<Expression*>*) override;
};

class ExecutionExpression : public Expression {
public:
    ExecutionExpression(size_t codeReference):Expression(codeReference){}
    static bool isExecution(list<Expression*>::iterator it);
    void interpret(list<Expression*>*) override;
};

class DefinitionExpression : public Expression {
public:
    DefinitionExpression(size_t codeReference):Expression(codeReference){}
    static bool isDefinition(list<Expression*>::iterator it);
    void interpret(list<Expression*>*) override;
};

class EqualExpression : public Expression {
public:
    EqualExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class EOEExpression : public Expression {
public:
    static bool isEOE(list<Expression*>::iterator it);
    EOEExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class SubstractionExpression : public Expression {
public:
    SubstractionExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class AdditionExpression : public Expression {
public:
    AdditionExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
    size_t getCodeReference();
};

class MultiplicationExpression : public Expression {
public:
    MultiplicationExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class DivitionExpression : public Expression {
public:
    DivitionExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class NegationExpression : public Expression {
public:
    NegationExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class OrExpression : public Expression {
public:
    OrExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class AndExpression : public Expression {
public:
    AndExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class MemberAccessExpression : public Expression {
public:
    MemberAccessExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class SeparatorExpression : public Expression {
public:
    SeparatorExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class OpenParenthesisExpression : public Expression {
public:
    OpenParenthesisExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class CloseParenthesisExpression : public Expression {
public:
    CloseParenthesisExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class OpenBracketExpression : public Expression {
public:
    OpenBracketExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class CloseBracketExpression : public Expression {
public:
    CloseBracketExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class OpenBraceExpression : public Expression {
public:
    OpenBraceExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class CloseBraceExpression : public Expression {
public:
    CloseBraceExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class StringExpression : public Expression {
public:
    ~StringExpression(){};
    StringExpression(size_t codeReference,string text):Expression(codeReference),_text(text){};
    void interpret(list<Expression*>*) override;
protected:
    string _text;
};

class NameExpression : public Expression {
public:
    NameExpression(size_t codeReference,string name):Expression(codeReference),_name(name) {};
    void interpret(list<Expression*>*) override;
    string _name;
};

class NullValueExpression : public Expression {
public:
    static bool isNull(list<Expression*>::iterator it);
    NullValueExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class ForExpression : public Expression {
public:
    static bool isFor(list<Expression*>::iterator it);
    ForExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class IfExpression : public Expression {
public:
    static bool isIf(list<Expression*>::iterator it);
    IfExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class BreakExpression : public Expression {
public:
    static bool isBreak(list<Expression*>::iterator it);
    BreakExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class ReturnExpression : public Expression {
public:
    ReturnExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class SampleTypeExpression : public Expression {
public:
    SampleTypeExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class SoundTypeExpression : public Expression {
public:
    SoundTypeExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class RealTypeExpression : public Expression {
public:
    RealTypeExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class IntegerTypeExpression : public Expression {
public:
    IntegerTypeExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class ArgumentTypeExpression : public Expression {
public:
    ArgumentTypeExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class GroupTypeExpression : public Expression {
public:
    GroupTypeExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class BoolTypeExpression : public Expression {
public:
    BoolTypeExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class BufferTypeExpression : public Expression {
public:
    BufferTypeExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class StringTypeExpression : public Expression {
public:
    StringTypeExpression(size_t codeReference):Expression(codeReference){};
    void interpret(list<Expression*>*) override;
};

class BooleanValueExpression : public Expression {
public:
    BooleanValueExpression(size_t codeReference,bool boolValue):Expression(codeReference),_boolValue(boolValue){};
    void interpret(list<Expression*>*) override;
protected:
    bool _boolValue;
};

class IntegerNumberExpression : public BooleanValueExpression {
public:
    IntegerNumberExpression(size_t codeReference,int integerNumber):BooleanValueExpression(codeReference,(bool)integerNumber), _integerNumber(integerNumber){};
    void interpret(list<Expression*>*) override;
protected:
    int _integerNumber;
};

class RealNumberExpression : public IntegerNumberExpression {
public:
    RealNumberExpression(size_t codeReference,int realNumber):IntegerNumberExpression(codeReference,(int)realNumber), _realNumber(realNumber){};
    void interpret(list<Expression*>*) override;
protected:
    double _realNumber;
};

}

#endif // EXPRESSION_H
