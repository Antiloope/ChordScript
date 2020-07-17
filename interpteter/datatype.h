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
    virtual bool executeMethod(string,LiteralValue*,LiteralValue*);
    virtual LiteralValue* cast(LiteralValue*) const = 0;
protected:
    unordered_map<string,void (*)(LiteralValue*,LiteralValue*)> _methods;
};

class SampleDataType : public DataType
{
public:
    SampleDataType();
    ~SampleDataType();
    LiteralValue* cast(LiteralValue*) const;
};

class SoundDataType : public DataType
{
public:
    SoundDataType();
    ~SoundDataType();
    LiteralValue* cast(LiteralValue*) const;
};

class NumericDataType : public DataType
{
public:
    NumericDataType();
    ~NumericDataType();
    LiteralValue* cast(LiteralValue*) const;
};

class ArgumentDataType : public DataType
{
public:
    ArgumentDataType();
    ~ArgumentDataType();
    LiteralValue* cast(LiteralValue*) const;
};

class GroupDataType : public DataType
{
public:
    GroupDataType();
    ~GroupDataType();
    LiteralValue* cast(LiteralValue*) const;
};

class BooleanDataType : public DataType
{
public:
    BooleanDataType();
    ~BooleanDataType();
    LiteralValue* cast(LiteralValue*) const;
};

class BufferDataType : public DataType
{
public:
    BufferDataType();
    ~BufferDataType();
    LiteralValue* cast(LiteralValue*) const;
};

class StringDataType : public DataType
{
public:
    StringDataType();
    ~StringDataType();
    LiteralValue* cast(LiteralValue*) const;
};

class NullDataType : public DataType
{
public:
    NullDataType();
    ~NullDataType();
    LiteralValue* cast(LiteralValue*) const;
};

class FunctionDataType : public DataType
{
public:
    FunctionDataType();
    ~FunctionDataType();
    LiteralValue* cast(LiteralValue*) const;
};

}

#endif // DATATYPE_H
