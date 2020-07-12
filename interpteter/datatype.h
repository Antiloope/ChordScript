#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>

using namespace std;

namespace CS {

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
};

class SampleDataType : public DataType
{

};

class SoundDataType : public DataType
{

};

class RealDataType : public DataType
{

};

class NumericDataType : public DataType
{

};

class ArgumentDataType : public DataType
{

};

class GroupDataType : public DataType
{

};

class BooleanDataType : public DataType
{

};

class BufferDataType : public DataType
{

};

class StringDataType : public DataType
{

};

class NullDataType : public DataType
{

};

class FunctionDataType : public DataType
{

};

}

#endif // DATATYPE_H
