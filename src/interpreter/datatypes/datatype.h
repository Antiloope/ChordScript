#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>
#include <unordered_map>

namespace CS {

class LiteralValue;
class ArgumentLiteralValue;

/**
 * This is the structure of a valid method function of derived data types.
 * Must return a LiteralValue pointer.
 * First argument is the variable name that the method is called.
 * Second argument is the LiteralValue of the variable with that name.
 * Third argument are the arguments passed to the method.
 */
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
     * @return Return the name of the data type as a string
     */
    static std::string getDataTypeString(DataTypesId);
    /**
     * @brief Utility method to get DataTypeId of a name
     * @return Return the DataTypeId of the data type name passed as string.
     * DataTypeId::Null if unknown
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
     * @return True if method was found and was executed. False ohterwise.
     */
    bool executeMethod(std::string,std::string,const LiteralValue*,const LiteralValue*);
    /**
     * @brief This method return a literal value casted to this (derived) data type.
     * This method is overriden in derived classes in order to implement casting.
     * @return A new literal value if variable must be casted. Or the same
     * LiteralValue* if cast is not necesary
     */
    virtual LiteralValue* cast(LiteralValue*) const = 0;
protected:
    std::unordered_map<std::string,method_function_t> _methods;
};

}

#include "argumentdatatype.h"
#include "arraydatatype.h"
#include "booleandatatype.h"
#include "groupdatatype.h"
#include "nulldatatype.h"
#include "numberdatatype.h"
#include "sampledatatype.h"
#include "sounddatatype.h"
#include "stringdatatype.h"

#endif // DATATYPE_H
