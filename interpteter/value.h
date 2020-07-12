#ifndef VALUE_H
#define VALUE_H

#include <string>
#include "terminalexpressions.h"
#include "utils/Exceptions/exception.h"
#include "datatype.h"
#include <tuple>

using namespace std;

namespace CS {

class Value
{
public:
    Value(DataTypesId);
    ~Value();
    DataTypesId getDataTypeId() const;
protected:
    DataTypesId _dataType;
};

class LiteralValue : public Value {
public:
    LiteralValue(DataTypesId,void*);
    ~LiteralValue();
    void* getValue() const {return _value;}
private:
    void* _value;
};

class LinkedValue : public Value {
public:
    LinkedValue();
    virtual LiteralValue* getValue() const = 0;
    virtual ~LinkedValue();
    static LinkedValue* generateLinkedValue(list<TerminalExpression*>*);
};

class StringLinkedValue : public LinkedValue {
public:
    StringLinkedValue(list<TerminalExpression*>*);
    LiteralValue * getValue() const override;
private:
    string _text;
};

class NullLinkedValue : public LinkedValue {
public:
    NullLinkedValue(list<TerminalExpression*>*);
    LiteralValue * getValue() const override;
};

class MathOperationLinkedValue : public LinkedValue {
public:
    MathOperationLinkedValue(list<TerminalExpression*>*);
    LiteralValue * getValue() const override;
private:
    enum Symbols: char {
        OpenParenthesis = 'a',
        Multiplication,
        Divition,
        Addition,
        Substraction
    };
    list<LinkedValue*> _linkedValuesList;
};

class BooleanOperationLinkedValue : public LinkedValue {
public:
    BooleanOperationLinkedValue(list<TerminalExpression*>*);
    ~BooleanOperationLinkedValue();
    LiteralValue * getValue() const override;
private:
    enum Symbols: char {
        OpenParenthesis = 'a',
        And,
        Or,
        Equal,
        Negation,
        NotEqual,
        Greater,
        GreaterEqual,
        Less,
        LessEqual
    };
    list<LinkedValue*> _linkedValuesList;
};

class ArrayLinkedValue : public LinkedValue {
public:
    ArrayLinkedValue(list<TerminalExpression*>*);
    LiteralValue * getValue() const override;
private:
    list<LinkedValue*> _linkedValuesList;
    DataTypesId _type;
};

class ExecutionLinkedValue : public LinkedValue {
public:
    ExecutionLinkedValue(list<TerminalExpression*>*);
    LiteralValue * getValue() const override;
private:
    string _name;
    list<tuple<string,ArrayLinkedValue*>> _methodsList;
};

class NameLinkedValue : public LinkedValue {
public:
    NameLinkedValue(list<TerminalExpression*>*);
    LiteralValue * getValue() const override;
private:
    string _name;
};

class NumericLinkedValue : public LinkedValue {
public:
    NumericLinkedValue(const double);
    LiteralValue * getValue() const override;
private:
    double _value;
};

class OperatorLinkedValue : public LinkedValue {
public:
    OperatorLinkedValue(char);
    LiteralValue * getValue() const override;
private:
    char _operator;
};

class BooleanLinkedValue : public LinkedValue {
public:
    BooleanLinkedValue(bool);
    LiteralValue * getValue() const override;
private:
    bool _value;
};

class BooleanOperatorLinkedValue : public LinkedValue {
public:
    BooleanOperatorLinkedValue(char);
    LiteralValue * getValue() const override;
private:
    char _operator;
};
}

#endif // VALUE_H
