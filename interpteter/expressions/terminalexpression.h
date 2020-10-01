#ifndef TERMINALEXPRESSION_H
#define TERMINALEXPRESSION_H

#include "expression.h"

namespace CS {

#define cCast(a) static_cast<char>(a)

enum struct TerminalExpressionType : char {
    Equal = 'a',
    EOE,
    Substract,
    Addition,
    Multiplication,
    Divition,
    Negation,
    Or,
    And,
    GreaterThan,
    LessThan,
    MemberAccess,
    Separator,
    OpenParenthesis,
    CloseParenthesis,
    OpenBracket,
    CloseBracket,
    CloseBrace,
    OpenBrace,
    String,
    Name,
    Null,
    For,
    If,
    Else,
    Break,
    Return,
    Boolean,
    Numeric
};

class TerminalExpression : public Expression {
public:
    TerminalExpression(size_t codeReference, char type) :
        Expression(codeReference), _type(type) {}

    TerminalExpression(const TerminalExpression& t) :
        Expression(t.getCodeReference()), _type(t.getType()) {}

    inline char getType() const { return _type; }
private:
    char _type;
};


class StringExpression : public TerminalExpression {
public:
    StringExpression(size_t codeReference, char type, std::string text) :
        TerminalExpression(codeReference,type),_text(text) {}

    StringExpression(const StringExpression& t) :
        TerminalExpression(t.getCodeReference(),t.getType()), _text(t.getText()) {}

    inline std::string getText() const { return _text; }
private:
    std::string _text;
};

class NameExpression : public TerminalExpression {
public:
    NameExpression(size_t codeReference, char type, std::string name) :
        TerminalExpression(codeReference,type),_name(name) {}

    NameExpression(const NameExpression& t) :
        TerminalExpression(t.getCodeReference(),t.getType()), _name(t.getName()) {}

    inline std::string getName() const { return _name; }
private:
    std::string _name;
};

class NumericExpression : public TerminalExpression {
public:
    NumericExpression(size_t codeReference, char type, double number) :
        TerminalExpression(codeReference,type),_value(number) {}

    NumericExpression(const NumericExpression& t) :
        TerminalExpression(t.getCodeReference(),t.getType()), _value(t.getValue()) {}

    inline double getValue() const { return _value; }
private:
    double _value;
};

class BooleanExpression : public TerminalExpression {
public:
    BooleanExpression(size_t codeReference, char type, bool value) :
        TerminalExpression(codeReference,type),_value(value) {}

    BooleanExpression(const BooleanExpression& t) :
        TerminalExpression(t.getCodeReference(),t.getType()), _value(t.getValue()) {}

    inline bool getValue() const { return _value; }
private:
    bool _value;
};

}

#endif // TERMINALEXPRESSION_H
