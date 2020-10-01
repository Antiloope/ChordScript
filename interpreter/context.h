#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <unordered_map>
#include <map>
#include <stack>
#include "executor/AudioFile.h"
#include "datatypes/datatype.h"
#include "languageConstants.h"

namespace CS {

// Defined in functiondefinition.h
class FunctionDefinition;
class BaseFunction;

typedef std::unordered_map<std::string,std::tuple<DataTypesId,LiteralValue*>> variables_map;
typedef std::unordered_map<std::string,std::tuple<DataTypesId,FunctionDefinition*>> functions_map;
typedef std::unordered_map<DataTypesId,DataType*> data_types_map;
typedef size_t scope_index;
typedef std::unordered_map<std::string,AudioFile<float>> audio_files_map;

const scope_index GLOBAL_SCOPE = 0;

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
    bool exist(std::string);

    /**
     * @brief getVariableDataType
     * @param name Variable name
     * @return
     */
    DataTypesId getVariableDataType(std::string);
    /**
     * @brief getDefinedDataType
     * @param name Variable name
     * @return
     */
    DataTypesId getDefinedDataType(std::string);
    /**
     * @brief getVariableValue
     * @param name Variable name
     * @return A pointer to a literal value stored in scope
     */
    LiteralValue* getVariableValue(std::string);
    /**
     * @brief setVariableValue
     * @param name Variable name
     * @param value LiteralValue
     * @return true if variable could be setted
     */
    bool setVariableValue(std::string,LiteralValue*);
    /**
     * @brief newVariable
     * @param name Variable name
     * @param dataType data type in variable definition
     */
    void newVariable(std::string, DataTypesId);

    /**
     * @brief this method is used to stop all sounds currently active
     * @return return a map with all playable variables
     */
    const variables_map getAllPlayables();

private:
    variables_map _scope;
};

typedef std::map<size_t,Scope> scopes_map;

/**
 * @brief Singleton class to sotre all values needed to be non volatile
 * on sucesive interpreter instances
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
    bool nameExist(std::string);
    /**
     * @brief functionNameExist
     * @param name Function name
     * @return true if exist a function with specified name
     */
    bool functionNameExist(std::string) const;
    /**
     * @brief isValidName
     * @param name Variable or function name
     * @return true if name its not a language keyword or anothe not valid names
     */
    bool isValidName(std::string) const;
    /**
     * @brief getVariableDataType
     * @param name Variable name
     * @return
     */
    DataTypesId getVariableDataType(std::string);

    /**
     * @brief newVariable
     * @param name Variable name
     * @param dataType data type in variable definition
     */
    void newVariable(std::string,DataTypesId);
    /**
     * @brief setVariableValue
     * @param name Variable name
     * @param value LiteralValue
     * @return true if variable could be setted
     */
    bool setVariableValue(std::string,LiteralValue*);
    /**
     * @brief getVariableValue
     * @param name Variable name
     * @return A pointer to a literal value stored in scope
     */
    LiteralValue* getVariableValue(std::string);
    /**
     * @brief getVariableValue
     * @param name Variable name
     * @param dataTypeId Id to a data type to cast
     * @return A pointer casted to specified data type from a literal value stored in scope
     */
    LiteralValue* getVariableValue(std::string,DataTypesId);
    /**
     * @brief getArgumentValue
     * @param name Argument name
     * @return A pointer to a literal value stored current scope
     */
    LiteralValue* getArgumentValue(std::string);
    /**
     * @brief getArgumentValue
     * @param name Argument name
     * @param dataTypeId Id to a data type to cast
     * @return A pointer casted to specified data type from a literal value stored current scope
     */
    LiteralValue* getArgumentValue(std::string,DataTypesId);

    /**
     * @brief newFunction
     * @param name Function name
     * @param dataType Function return data type
     * @return A pointer to a new function definition. Destruction responsability is over this Context class
     */
    FunctionDefinition* newFunction(std::string,DataTypesId);
    /**
     * @brief executeFunction
     * @param name Function name
     * @param arguments Literal arguments to execute function
     * @return true if function could be executed correctly
     */
    bool executeFunction(std::string,LiteralValue*);

    /**
     * @brief executeMethod
     * @param variableName
     * @param methodName
     * @param arguments
     * @return true if method could be executed correctly
     */
    bool executeMethod(std::string,std::string,LiteralValue*);

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
    /**
     * @brief this method is used to stop all sounds currently active
     * To do it, this method iterate over all scopes searching for playables.
     * @return return a map with all playable variables.
     */
    const variables_map getAllPlayables();

    /**
     * @brief This method store an AudioFile in memory, in order to be
     * used later as a sample.
     * @param fileName (unique)
     * @param audioFile
     */
    void newAudioFile(std::string,AudioFile<float>);
    /**
     * @brief removeAudioFile
     * @param fileName
     */
    void removeAudioFile(std::string);
    /**
     * @brief getAudioFile
     * @param fileName
     * @return
     */
    const AudioFile<float>* getAudioFile(std::string) const;

    /**
     * @brief This method store a copy of the value passed as argument.
     * @param returnValue
     */
    void setReturnValue(LiteralValue*);
    /**
     * @brief This method store a CS::NullLiteralValue as return value.
     */
    void setReturnValue();
    /**
     * @brief getReturnValue
     * @return A copy of the return value internaly stored.
     * Delegates responsability to free this pointer later.
     */
    LiteralValue* getReturnValue() const;
    /**
     * @brief Check if there are a return value setted.
     * @return false if return value is a nullptr, true otherwise.
     */
    bool existReturnValue() const;
private:
    static Context* _instance;
    Context();

    bool executeVariableFunction(std::string,LiteralValue*);

    std::stack<scope_index> _freeScopesIndexes;
    std::stack<scope_index> _scopeStack;
    data_types_map _dataTypes;
    scopes_map _scopes;
    functions_map _functions;
    LiteralValue* _returnValue;
    audio_files_map _audioFiles;

    BaseFunction* newBaseFunction(std::string,DataTypesId);
};

}

#endif // CONTEXT_H
