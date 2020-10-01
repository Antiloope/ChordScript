#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>
#include <unordered_map>

namespace CS {

class LiteralValue;
class ArgumentLiteralValue;

typedef LiteralValue* (*methodFunction_t)(const LiteralValue*,const LiteralValue*);

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
    static std::string getDataTypeString(DataTypesId);
    static DataTypesId getDataTypeId(std::string);
    DataType();
    virtual ~DataType();
    bool executeMethod(std::string,const LiteralValue*,const LiteralValue*);
    virtual LiteralValue* cast(LiteralValue*) const = 0;
protected:
    std::unordered_map<std::string,methodFunction_t> _methods;
};

}

#include "argumentdatatype.h"
#include "booleandatatype.h"
#include "groupdatatype.h"
#include "nulldatatype.h"
#include "numberdatatype.h"
#include "sampledatatype.h"
#include "sounddatatype.h"
#include "stringdatatype.h"

#endif // DATATYPE_H
