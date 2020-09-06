#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>
#include <unordered_map>

using namespace std;

namespace CS {

class LiteralValue;
class ArgumentLiteralValue;

typedef LiteralValue* (*methodFunction_t)(LiteralValue*,LiteralValue*);

enum class DataTypesId {
    Sample,
    Sound,
    Numeric,
    Argument,
    Operator,
    Array,
    Group,
    Boolean,
    String,
    Null,
};

class DataType
{
public:
    static string getDataTypeString(DataTypesId);
    static DataTypesId getDataTypeId(string);
    DataType();
    virtual ~DataType();
    bool executeMethod(string,LiteralValue*,LiteralValue*);
    virtual LiteralValue* cast(LiteralValue*) const = 0;
protected:
    unordered_map<string,methodFunction_t> _methods;
};

class SampleDataType : public DataType
{
public:
    SampleDataType();
    ~SampleDataType();
    LiteralValue* cast(LiteralValue*) const;
private:
    static LiteralValue* play(LiteralValue*,LiteralValue*);
    static LiteralValue* setPanning(LiteralValue*,LiteralValue*);
};

class SoundDataType : public DataType
{
public:
    SoundDataType();
    ~SoundDataType();
    LiteralValue* cast(LiteralValue*) const;
private:
    static LiteralValue* play(LiteralValue*,LiteralValue*);
    static LiteralValue* setPanning(LiteralValue*,LiteralValue*);
    static LiteralValue* constantFreq(LiteralValue*,LiteralValue*);
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

}

#endif // DATATYPE_H
