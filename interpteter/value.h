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
    LiteralValue(DataTypesId);
    LiteralValue(const LiteralValue&);
    virtual ~LiteralValue();
    void* getValue() const {return _value;}
protected:
    void* _value;
};

class StringLiteralValue : public LiteralValue {
public:
    StringLiteralValue(string);
    ~StringLiteralValue();
private:
    string _text;
};

class NumericLiteralValue : public LiteralValue {
public:
    NumericLiteralValue(double);
    ~NumericLiteralValue();
private:
    double _number;
};

class BooleanLiteralValue : public LiteralValue {
public:
    BooleanLiteralValue(bool);
    ~BooleanLiteralValue();
private:
    bool _boolean;
};

class NullLiteralValue : public LiteralValue {
public:
    NullLiteralValue();
    ~NullLiteralValue();
};

class OperatorLiteralValue : public LiteralValue {
public:
    OperatorLiteralValue(char);
    ~OperatorLiteralValue();
private:
    char _operator;
};

class LinkedValue : public Value {
public:
    LinkedValue(size_t);
    virtual LiteralValue* getValue() const = 0;
    virtual void load(list<TerminalExpression*>*) = 0;
    virtual ~LinkedValue();
    static LinkedValue* generateLinkedValue(list<TerminalExpression*>*);
    size_t getCodeReference() const;
private:
    size_t _codeReference;
};

class StringLinkedValue : public LinkedValue {
public:
    StringLinkedValue(size_t);
    void load(list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    string _text;
};

class NullLinkedValue : public LinkedValue {
public:
    NullLinkedValue(size_t);
    void load(list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
};

enum class MathSymbols : char{
    OpenParenthesis = 'a',
    Multiplication,
    Divition,
    Addition,
    Substraction
};

enum class BooleanSymbols : char {
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

class MathOperationLinkedValue : public LinkedValue {
public:
    MathOperationLinkedValue(size_t);
    void load(list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    list<LinkedValue*> _linkedValuesList;
};

class BooleanOperationLinkedValue : public LinkedValue {
public:
    BooleanOperationLinkedValue(size_t);
    ~BooleanOperationLinkedValue();
    void load(list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    list<LinkedValue*> _linkedValuesList;
};

class ArrayLinkedValue : public LinkedValue {
public:
    ArrayLinkedValue(size_t);
    void load(list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    list<LinkedValue*> _linkedValuesList;
    DataTypesId _type;
};

class ExecutionLinkedValue : public LinkedValue {
public:
    ExecutionLinkedValue(size_t);
    void load(list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    string _name;
    list<tuple<string,ArrayLinkedValue*>> _methodsList;
};

class NameLinkedValue : public LinkedValue {
public:
    NameLinkedValue(size_t);
    void load(list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    string _name;
};

class NumericLinkedValue : public LinkedValue {
public:
    NumericLinkedValue(size_t);
    void load(list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    double _value;
};

class OperatorLinkedValue : public LinkedValue {
public:
    OperatorLinkedValue(size_t);
    void load(list<TerminalExpression *> *) override;
    void load(char);
    LiteralValue * getValue() const override;
private:
    char _operator;
};

class BooleanLinkedValue : public LinkedValue {
public:
    BooleanLinkedValue(size_t);
    void load(list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    bool _value;
};

}

#endif // VALUE_H
