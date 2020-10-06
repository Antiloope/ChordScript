#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>
#include <unordered_map>

namespace CS {

class LiteralValue;
class ArgumentLiteralValue;

// This is the type of functions that a method could be.
// first argument is the value of the called variable method and
// the second argument are the parameters wich the method are called.
typedef LiteralValue* (*method_function_t)(std::string,const LiteralValue*,const LiteralValue*);

enum class DataTypesId {
    Sample,
    Sound,
    Numeric,
    Argument,
    Operator, // Only for internal usage
    Array,
    Group,
    Boolean,
    String,
    Null,
};

class DataType
{
public:
    /**
     * @brief Utility method to get the name of a data type
     * @return Return the name of the data type in the language
     */
    static std::string getDataTypeString(DataTypesId);
    /**
     * @brief Utility method to get DataTypeId of a name
     * @return Return the DataTypeId of the data type name passed as string
     */
    static DataTypesId getDataTypeId(std::string);
    DataType();
    virtual ~DataType();
    /**
     * @brief This method execute a method of the current (derived) data type
     * @param methodName
     * @param variableName
     * @param value
     * @param arguments
     * @return true if method was found and was executed
     */
    bool executeMethod(std::string,std::string,const LiteralValue*,const LiteralValue*);
    /**
     * @brief This method return a literal value casted to this (derived) data type
     * This method is overriden in derived classes in order to implement casting
     * @return A new literal value if variable must be casted. Or the same
     * literal value if cast is not necesary
     */
    virtual LiteralValue* cast(LiteralValue*) const = 0;
protected:
    std::unordered_map<std::string,method_function_t> _methods;
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
