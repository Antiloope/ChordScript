#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <stack>
#include <tuple>

#include "value.h"
#include "datatype.h"
#include "languageConstants.h"

using namespace std;
namespace CS {

class FunctionDefinition;
class BaseFunction;

typedef unordered_map<string,tuple<DataTypesId,LiteralValue*>> variables_map;
typedef unordered_map<string,tuple<DataTypesId,FunctionDefinition*>> functions_map;
typedef unordered_map<DataTypesId,DataType*> data_types_map;
typedef size_t scope_index;

const scope_index GlobalScope = 0;
const scope_index MaxContextCount = 1000;

/**
 * @brief Axiliar class to manage variables on each scope
 */
class Scope
{
public:
    Scope();
    Scope(const Scope&);
    ~Scope();

    /**
     * @brief Check if variable exist on this scope
     * @param name Variable name
     * @return true if exist
     */
    bool exist(string);

    /**
     * @brief getVariableDataType
     * @param name Variable name
     * @return
     */
    DataTypesId getVariableDataType(string);
    /**
     * @brief getDefinedDataType
     * @param name Variable name
     * @return
     */
    DataTypesId getDefinedDataType(string);
    /**
     * @brief getVariableValue
     * @param name Variable name
     * @return A pointer to a literal value stored in scope
     */
    LiteralValue* getVariableValue(string);
    /**
     * @brief setVariableValue
     * @param name Variable name
     * @param value LiteralValue
     * @return true if variable could be setted
     */
    bool setVariableValue(string,LiteralValue*);
    /**
     * @brief newVariable
     * @param name Variable name
     * @param dataType data type in variable definition
     */
    void newVariable(string, DataTypesId);

private:
    variables_map _scope;
};

typedef map<size_t,Scope> scopes_map;

/**
 * @brief Singleton class to sotre all values needed to be non volatile on sucesive interpreter instances
 */
class Context
{
public:
    static Context* getInstance();
    /**
     * @brief load initial values.
     */
    void load();
    /**
     * @brief isDataType
     * @return true if its an instantiable data type
     */
    bool isDataType(DataTypesId) const;
    /**
     * @brief nameExist
     * @param name Variable name
     * @return true if exist a variable with specified name
     */
    bool nameExist(string);
    /**
     * @brief functionNameExist
     * @param name Function name
     * @return true if exist a function with specified name
     */
    bool functionNameExist(string) const;
    /**
     * @brief isValidName
     * @param name Variable or function name
     * @return true if name its not a language keyword or anothe not valid names
     */
    bool isValidName(string) const;
    /**
     * @brief getVariableDataType
     * @param name Variable name
     * @return
     */
    DataTypesId getVariableDataType(string);

    /**
     * @brief newVariable
     * @param name Variable name
     * @param dataType data type in variable definition
     */
    void newVariable(string,DataTypesId);
    /**
     * @brief setVariableValue
     * @param name Variable name
     * @param value LiteralValue
     * @return true if variable could be setted
     */
    bool setVariableValue(string,LiteralValue*);
    /**
     * @brief getVariableValue
     * @param name Variable name
     * @return A pointer to a literal value stored in scope
     */
    LiteralValue* getVariableValue(string);
    /**
     * @brief getVariableValue
     * @param name Variable name
     * @param dataTypeId Id to a data type to cast
     * @return A pointer casted to specified data type from a literal value stored in scope
     */
    LiteralValue* getVariableValue(string,DataTypesId);
    /**
     * @brief getArgumentValue
     * @param name Argument name
     * @return A pointer to a literal value stored current scope
     */
    LiteralValue* getArgumentValue(string);
    /**
     * @brief getArgumentValue
     * @param name Argument name
     * @param dataTypeId Id to a data type to cast
     * @return A pointer casted to specified data type from a literal value stored current scope
     */
    LiteralValue* getArgumentValue(string,DataTypesId);

    /**
     * @brief newFunction
     * @param name Function name
     * @param dataType Function return data type
     * @return A pointer to a new function definition. Destruction responsability is over this Context class
     */
    FunctionDefinition* newFunction(string,DataTypesId);
    /**
     * @brief executeFunction
     * @param name Function name
     * @param arguments Literal arguments to execute function
     * @return true if function could be executed correctly
     */
    bool executeFunction(string,LiteralValue*);

    /**
     * @brief executeMethod
     * @param variableName
     * @param methodName
     * @param arguments
     * @return true if method could be executed correctly
     */
    bool executeMethod(string,string,LiteralValue*);

    /**
     * @brief newScope
     * @return Index to new scope. Usefull to switch later.
     */
    scope_index newScope();
    /**
     * @brief returnScope Used to return to previous scope in definitions.
     */
    void returnScope();
    /**
     * @brief getCurrentScope
     * @return Index to current scope
     */
    scope_index getCurrentScope() const;
    /**
     * @brief switchScope Creates a new scope as a copy of the scope passed as argument and set current scope tho this
     * @param scopeIndex
     * @return Index to new copy scope to be removed when execution is finished.
     */
    scope_index switchScope(scope_index);
    /**
     * @brief removeScope remove scope and all definitions inside it.
     * @param scopeIndex
     */
    void removeScope(scope_index);

    void setReturnValue(LiteralValue*);
    LiteralValue* getReturnValue() const;
private:
    static Context* _instance;
    Context();
    stack<scope_index> _freeScopesIndexes;
    stack<scope_index> _scopeStack;
    data_types_map _dataTypes;
    scopes_map _scopes;
    functions_map _functions;
    LiteralValue* _returnValue;

    BaseFunction* newBaseFunction(string,DataTypesId);
};

}

#endif // CONTEXT_H
