#ifndef VALUE_H
#define VALUE_H

#include <string>
#include "terminalexpressions.h"
#include "utils/Exceptions/exception.h"

using namespace std;

namespace CS {

class Value
{
public:
    Value(string dataType);
    ~Value();
    string getDataTypeName() const;
protected:
    string _dataType;
};

class LiteralValue : public Value {
public:
    LiteralValue(string dataType);
};

class LinkedValue : public Value {
public:
    LinkedValue();
    virtual LiteralValue* getValue() const = 0;
};

class StringLiteralValue : public LiteralValue {
public:
    StringLiteralValue(string);
    string getValue() const;
private:
    string _text;
};

class StringLinkedValue : public LinkedValue {
public:
    StringLinkedValue(const StringExpression*);
    LiteralValue * getValue() const override;
private:
    string _text;
};

class NullLiteralValue : public LiteralValue {
public:
    NullLiteralValue();
};

class NullLinkedValue : public LinkedValue {
public:
    NullLinkedValue();
    LiteralValue * getValue() const override;
};

class MathOperationLinkedValue : public LinkedValue {
public:
    MathOperationLinkedValue(list<TerminalExpression*>*);
    LiteralValue * getValue() const override;
private:
    list<LinkedValue*> _linkedValuesList;
};

class BooleanOperationLinkedValue : public LinkedValue {
public:
    BooleanOperationLinkedValue(list<TerminalExpression*>*);
    LiteralValue * getValue() const override;
private:
    list<LinkedValue*> _linkedValuesList;
};

class ArrayLinkedValue : public LinkedValue {
public:
    ArrayLinkedValue(list<TerminalExpression*>*);
    LiteralValue * getValue() const override;
private:
    list<LinkedValue*> _linkedValuesList;
    string _type;
};

class ExecutionLinkedValue : public LinkedValue {
public:
    ExecutionLinkedValue(list<TerminalExpression*>*);
    LiteralValue * getValue() const override;
};

class NameLinkedValue : public LinkedValue {
public:
    NameLinkedValue(NameExpression*);
    LiteralValue * getValue() const override;
private:
    string _name;
};

class NumericLiteralValue : public LiteralValue {
public:
    NumericLiteralValue(double);
    double getValue() const;
private:
    double _value;
};

class NumericLinkedValue : public LinkedValue {
public:
    NumericLinkedValue(const double);
    LiteralValue * getValue() const override;
private:
    double _value;
};

class OperatorLiteralValue : public LiteralValue {
public:
    OperatorLiteralValue(char);
    double getValue(double, double) const;
private:
    char _operator;
};

class OperatorLinkedValue : public LinkedValue {
public:
    OperatorLinkedValue(char);
    LiteralValue * getValue() const override;
private:
    char _operator;
};

class BooleanLiteralValue : public LiteralValue {
public:
    BooleanLiteralValue(bool);
    bool getValue() const;
private:
    bool _value;
};

class BooleanLinkedValue : public LinkedValue {
public:
    BooleanLinkedValue(bool);
    LiteralValue * getValue() const override;
private:
    bool _value;
};

class BooleanOperatorLiteralValue : public LiteralValue {
public:
    BooleanOperatorLiteralValue(char);
    bool getValue(bool,bool) const;
    bool getValue(double,double) const;
    bool getValue(string,string) const;
private:
    char _operator;
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
