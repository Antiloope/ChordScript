#ifndef VALUE_H
#define VALUE_H

#include <string>
#include "terminalexpressions.h"
#include "utils/Exceptions/exception.h"
#include "datatypes/datatype.h"
#include "soundgenerator.h"
#include "sampleplayer.h"
#include <tuple>
#include "executor/executorinterface.h"

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
    void* getValue() const { return _value; }
    virtual LiteralValue* clone() = 0;
protected:
    void* _value;
};

class StringLiteralValue : public LiteralValue {
public:
    StringLiteralValue(string);
    LiteralValue* clone();
    ~StringLiteralValue();
private:
    string _text;
};

class NumericLiteralValue : public LiteralValue {
public:
    NumericLiteralValue(double);
    LiteralValue* clone();
    ~NumericLiteralValue();
private:
    double _number;
};

class BooleanLiteralValue : public LiteralValue {
public:
    BooleanLiteralValue(bool);
    LiteralValue* clone();
    ~BooleanLiteralValue();
private:
    bool _boolean;
};

class NullLiteralValue : public LiteralValue {
public:
    NullLiteralValue();
    LiteralValue* clone();
    ~NullLiteralValue();
};

class OperatorLiteralValue : public LiteralValue {
public:
    OperatorLiteralValue(char);
    LiteralValue* clone();
    ~OperatorLiteralValue();
private:
    char _operator;
};

class ArrayLiteralValue : public LiteralValue {
public:
    ArrayLiteralValue(list<LiteralValue*>);
    LiteralValue* clone();
    ~ArrayLiteralValue();
private:
    list<LiteralValue*> _literalValuesList;
};

class ArgumentLiteralValue : public LiteralValue {
public:
    ArgumentLiteralValue(list<LiteralValue*>);
    LiteralValue* clone();
    ~ArgumentLiteralValue();
private:
    list<LiteralValue*> _literalValuesList;
};

class SoundLiteralValue : public LiteralValue {
public:
    SoundLiteralValue(SoundGenerator*);
    LiteralValue* clone();
    ~SoundLiteralValue();
private:
    SoundGenerator* _soundGenerator;
};

class SampleLiteralValue : public LiteralValue {
public:
    SampleLiteralValue(SamplePlayer*);
    LiteralValue* clone();
    ~SampleLiteralValue();
private:
    SamplePlayer* _samplePlayer;
};

class LinkedValue : public Value {
public:
    LinkedValue(size_t);
    virtual LiteralValue* getValue() const = 0;
    virtual void load(list<TerminalExpression*>*) = 0;
    virtual ~LinkedValue();
    static LinkedValue* generateLinkedValue(list<TerminalExpression*>*);
    static LinkedValue* detectOperation(list<TerminalExpression*>*);
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
    ~MathOperationLinkedValue();
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
    ~ArrayLinkedValue();
    void load(list<TerminalExpression *> *) override;
    LiteralValue * getValue() const override;
private:
    list<LinkedValue*> _linkedValuesList;
    DataTypesId _type;
};

class ExecutionLinkedValue : public LinkedValue {
public:
    ExecutionLinkedValue(size_t);
    ~ExecutionLinkedValue();
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
