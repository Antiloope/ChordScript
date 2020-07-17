#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>
#include <unordered_map>

using namespace std;

namespace CS {

class LiteralValue;
class ArgumentLiteralValue;

enum class DataTypesId {
    Sample,
    Sound,
    Numeric,
    Argument,
    Operator,
    Array,
    Group,
    Boolean,
    Buffer,
    String,
    Null,
    Function
};

class DataType
{
public:
    static string getDataTypeString(DataTypesId);
    static DataTypesId getDataTypeId(string);
    DataType();
    virtual ~DataType();
    virtual bool executeMethod(string,LiteralValue*,LiteralValue*) = 0;
    virtual LiteralValue* cast(LiteralValue*) const = 0;
protected:
    unordered_map<string,void (*)(LiteralValue*,LiteralValue*)> _methods;
};

class SampleDataType : public DataType
{
public:
    SampleDataType();
    ~SampleDataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    LiteralValue* cast(LiteralValue*) const;
};

class SoundDataType : public DataType
{
public:
    SoundDataType();
    ~SoundDataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    LiteralValue* cast(LiteralValue*) const;
};

class NumericDataType : public DataType
{
public:
    NumericDataType();
    ~NumericDataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    LiteralValue* cast(LiteralValue*) const;
};

class ArgumentDataType : public DataType
{
public:
    ArgumentDataType();
    ~ArgumentDataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    LiteralValue* cast(LiteralValue*) const;
};

class GroupDataType : public DataType
{
public:
    GroupDataType();
    ~GroupDataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    LiteralValue* cast(LiteralValue*) const;
};

class BooleanDataType : public DataType
{
public:
    BooleanDataType();
    ~BooleanDataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    LiteralValue* cast(LiteralValue*) const;
};

class BufferDataType : public DataType
{
public:
    BufferDataType();
    ~BufferDataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    LiteralValue* cast(LiteralValue*) const;
};

class StringDataType : public DataType
{
public:
    StringDataType();
    ~StringDataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    LiteralValue* cast(LiteralValue*) const;
};

class NullDataType : public DataType
{
public:
    NullDataType();
    ~NullDataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    LiteralValue* cast(LiteralValue*) const;
};

class FunctionDataType : public DataType
{
public:
    FunctionDataType();
    ~FunctionDataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // DATATYPE_H
