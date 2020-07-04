#ifndef VALUE_H
#define VALUE_H

#include <string>
#include "terminalexpressions.h"
#include "utils/Exceptions/exception.h"
#include <tuple>

using namespace std;

namespace CS {

class Value
{
public:
    Value(string);
    ~Value();
    string getDataTypeName() const;
protected:
    string _dataType;
};

class LiteralValue : public Value {
public:
    LiteralValue(string,void*);
    ~LiteralValue();
    void* getValue() const {return _value;}
private:
    void* _value;
};

class LinkedValue : public Value {
public:
    LinkedValue();
    virtual LiteralValue* getValue() const = 0;
};

class StringLinkedValue : public LinkedValue {
public:
    StringLinkedValue(const StringExpression*);
    LiteralValue * getValue() const override;
private:
    string _text;
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
private:
    string _name;
    list<tuple<string,ArrayLinkedValue*>> _methodsList;
};

class NameLinkedValue : public LinkedValue {
public:
    NameLinkedValue(NameExpression*);
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
