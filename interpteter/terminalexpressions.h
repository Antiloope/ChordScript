#ifndef TERMINALEXPRESSION_H
#define TERMINALEXPRESSION_H

#include "expression.h"

namespace CS {

class TerminalExpression : public Expression {
public:
    TerminalExpression(size_t codeReference) : Expression(codeReference){}
    TerminalExpression(const TerminalExpression&){}
};

class EqualExpression : public TerminalExpression {
public:
    EqualExpression(size_t codeReference) : TerminalExpression(codeReference){};
    EqualExpression(const EqualExpression& expression) : TerminalExpression(expression){}
};

class EOEExpression : public TerminalExpression {
public:
    EOEExpression(size_t codeReference) : TerminalExpression(codeReference){};
    EOEExpression(const EOEExpression& expression) : TerminalExpression(expression){}
};

class SubstractionExpression : public TerminalExpression {
public:
    SubstractionExpression(size_t codeReference) : TerminalExpression(codeReference){};
    SubstractionExpression(const SubstractionExpression& expression) : TerminalExpression(expression){}
};

class AdditionExpression : public TerminalExpression {
public:
    AdditionExpression(size_t codeReference) : TerminalExpression(codeReference){};
    AdditionExpression(const AdditionExpression& expression) : TerminalExpression(expression){}
};

class MultiplicationExpression : public TerminalExpression {
public:
    MultiplicationExpression(size_t codeReference) : TerminalExpression(codeReference){};
    MultiplicationExpression(const MultiplicationExpression& expression) : TerminalExpression(expression){}
};

class DivitionExpression : public TerminalExpression {
public:
    DivitionExpression(size_t codeReference) : TerminalExpression(codeReference){};
    DivitionExpression(const DivitionExpression& expression) : TerminalExpression(expression){}
};

class NegationExpression : public TerminalExpression {
public:
    NegationExpression(size_t codeReference) : TerminalExpression(codeReference){};
    NegationExpression(const NegationExpression& expression) : TerminalExpression(expression){}
};

class OrExpression : public TerminalExpression {
public:
    OrExpression(size_t codeReference) : TerminalExpression(codeReference){};
    OrExpression(const OrExpression& expression) : TerminalExpression(expression){}
};

class AndExpression : public TerminalExpression {
public:
    AndExpression(size_t codeReference) : TerminalExpression(codeReference){};
    AndExpression(const AndExpression& expression) : TerminalExpression(expression){}
};

class GreaterThanExpression : public TerminalExpression {
public:
    GreaterThanExpression(size_t codeReference) : TerminalExpression(codeReference){};
    GreaterThanExpression(const GreaterThanExpression& expression) : TerminalExpression(expression){}
};

class LessThanExpression : public TerminalExpression {
public:
    LessThanExpression(size_t codeReference) : TerminalExpression(codeReference){};
    LessThanExpression(const LessThanExpression& expression) : TerminalExpression(expression){}
};

class MemberAccessExpression : public TerminalExpression {
public:
    MemberAccessExpression(size_t codeReference) : TerminalExpression(codeReference){};
    MemberAccessExpression(const MemberAccessExpression& expression) : TerminalExpression(expression){}
};

class SeparatorExpression : public TerminalExpression {
public:
    SeparatorExpression(size_t codeReference) : TerminalExpression(codeReference){};
    SeparatorExpression(const SeparatorExpression& expression) : TerminalExpression(expression){}
};

class OpenParenthesisExpression : public TerminalExpression {
public:
    OpenParenthesisExpression(size_t codeReference) : TerminalExpression(codeReference){};
    OpenParenthesisExpression(const OpenParenthesisExpression& expression) : TerminalExpression(expression){}
};

class CloseParenthesisExpression : public TerminalExpression {
public:
    CloseParenthesisExpression(size_t codeReference) : TerminalExpression(codeReference){};
    CloseParenthesisExpression(const CloseParenthesisExpression& expression) : TerminalExpression(expression){}
};

class OpenBracketExpression : public TerminalExpression {
public:
    OpenBracketExpression(size_t codeReference) : TerminalExpression(codeReference){};
    OpenBracketExpression(const OpenBracketExpression& expression) : TerminalExpression(expression){}
};

class CloseBracketExpression : public TerminalExpression {
public:
    CloseBracketExpression(size_t codeReference) : TerminalExpression(codeReference){};
    CloseBracketExpression(const CloseBracketExpression& expression) : TerminalExpression(expression){}
};

class OpenBraceExpression : public TerminalExpression {
public:
    OpenBraceExpression(size_t codeReference) : TerminalExpression(codeReference){};
    OpenBraceExpression(const OpenBraceExpression& expression) : TerminalExpression(expression){}
};

class CloseBraceExpression : public TerminalExpression {
public:
    CloseBraceExpression(size_t codeReference) : TerminalExpression(codeReference){};
    CloseBraceExpression(const CloseBraceExpression& expression) : TerminalExpression(expression){}
};

class StringExpression : public TerminalExpression {
public:
    StringExpression(size_t codeReference, string text) : TerminalExpression(codeReference), _text(text){};
    StringExpression(const StringExpression& expression) : TerminalExpression(expression), _text(expression._text){}
    string getText() const {return _text;}
protected:
    string _text;
};

class NameExpression : public TerminalExpression {
public:
    NameExpression(size_t codeReference, string name) : TerminalExpression(codeReference), _name(name){};
    NameExpression(const NameExpression& expression) : TerminalExpression(expression), _name(expression._name){}
    string getName() const {return _name;}
protected:
    string _name;
};

class NullValueExpression : public TerminalExpression {
public:
    NullValueExpression(size_t codeReference) : TerminalExpression(codeReference){};
    NullValueExpression(const NullValueExpression& expression) : TerminalExpression(expression){}
};

class ForExpression : public TerminalExpression {
public:
    ForExpression(size_t codeReference) : TerminalExpression(codeReference){};
    ForExpression(const ForExpression& expression) : TerminalExpression(expression){}
};

class IfExpression : public TerminalExpression {
public:
    IfExpression(size_t codeReference) : TerminalExpression(codeReference){};
    IfExpression(const IfExpression& expression) : TerminalExpression(expression){}
};

class BreakExpression : public TerminalExpression {
public:
    BreakExpression(size_t codeReference) : TerminalExpression(codeReference){};
    BreakExpression(const BreakExpression& expression) : TerminalExpression(expression){}
};

class ReturnExpression : public TerminalExpression {
public:
    ReturnExpression(size_t codeReference) : TerminalExpression(codeReference){};
    ReturnExpression(const ReturnExpression& expression) : TerminalExpression(expression){}
};

class SampleTypeExpression : public TerminalExpression {
public:
    SampleTypeExpression(size_t codeReference) : TerminalExpression(codeReference){};
    SampleTypeExpression(const SampleTypeExpression& expression) : TerminalExpression(expression){}
};

class SoundTypeExpression : public TerminalExpression {
public:
    SoundTypeExpression(size_t codeReference) : TerminalExpression(codeReference){};
    SoundTypeExpression(const SoundTypeExpression& expression) : TerminalExpression(expression){}
};

class RealTypeExpression : public TerminalExpression {
public:
    RealTypeExpression(size_t codeReference) : TerminalExpression(codeReference){};
    RealTypeExpression(const RealTypeExpression& expression) : TerminalExpression(expression){}
};

class IntegerTypeExpression : public TerminalExpression {
public:
    IntegerTypeExpression(size_t codeReference) : TerminalExpression(codeReference){};
    IntegerTypeExpression(const IntegerTypeExpression& expression) : TerminalExpression(expression){}
};

class ArgumentTypeExpression : public TerminalExpression {
public:
    ArgumentTypeExpression(size_t codeReference) : TerminalExpression(codeReference){};
    ArgumentTypeExpression(const ArgumentTypeExpression& expression) : TerminalExpression(expression){}
};

class GroupTypeExpression : public TerminalExpression {
public:
    GroupTypeExpression(size_t codeReference) : TerminalExpression(codeReference){};
    GroupTypeExpression(const GroupTypeExpression& expression) : TerminalExpression(expression){}
};

class BoolTypeExpression : public TerminalExpression {
public:
    BoolTypeExpression(size_t codeReference) : TerminalExpression(codeReference){};
    BoolTypeExpression(const BoolTypeExpression& expression) : TerminalExpression(expression){}
};

class BufferTypeExpression : public TerminalExpression {
public:
    BufferTypeExpression(size_t codeReference) : TerminalExpression(codeReference){};
    BufferTypeExpression(const BufferTypeExpression& expression) : TerminalExpression(expression){}
};

class StringTypeExpression : public TerminalExpression {
public:
    StringTypeExpression(size_t codeReference) : TerminalExpression(codeReference){};
    StringTypeExpression(const StringTypeExpression& expression) : TerminalExpression(expression){}
};

class NumericValueExpression : public TerminalExpression {
public:
    NumericValueExpression(size_t codeReference, int realValue) : TerminalExpression(codeReference), _value(realValue) {};
    NumericValueExpression(const NumericValueExpression& expression) : TerminalExpression(expression), _value(expression._value){}
    double getValue() const {return _value;}
protected:
    double _value;
};

class BooleanValueExpression : public TerminalExpression {
public:
    BooleanValueExpression(size_t codeReference, bool boolValue) : TerminalExpression(codeReference), _boolValue(boolValue) {};
    BooleanValueExpression(const BooleanValueExpression& expression) : TerminalExpression(expression), _boolValue(expression._boolValue){}
    BooleanValueExpression(const NumericValueExpression& expression) : TerminalExpression(expression), _boolValue((bool)expression.getValue()){}
    bool getBoolValue() const {return _boolValue;}
protected:
    bool _boolValue;
};

}

#endif // TERMINALEXPRESSION_H
