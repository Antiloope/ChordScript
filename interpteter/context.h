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

using namespace std;
namespace CS {

class FunctionDefinition;

typedef unordered_map<string,tuple<string,LiteralValue*>> variables_map;
typedef map<size_t,variables_map> context_map;
typedef unordered_map<string,tuple<string,FunctionDefinition*>> functions_map;
typedef unordered_map<string,DataType*> data_types_map;
typedef unordered_set<string> reserved_keywords_map;
typedef size_t context_index;

const context_index GlobalContext = 0;
const context_index UnknownContext = 4294967295;

class Context
{
public:
    static Context* getInstance();

    bool isDataType(string) const;
    bool nameExist(string);
    bool functionNameExist(string) const;
    bool isValidName(string) const;
    string getDataTypeName(string);

    void newVariable(string,string);
    bool setVariableValue(string,LiteralValue*);
    void newFunction(string,string,FunctionDefinition*);

    context_index newContext();
    void returnContext();
    context_index getCurrentContext() const;

    void removeContext(context_index);

    void setReturnValue(LiteralValue*);
    LiteralValue* getReturnValue() const;
private:
    static Context* _instance;
    Context();
    stack<context_index> _contextStack;
    data_types_map _dataTypes;
    reserved_keywords_map _reservedKeywords;
    context_map _variables;
    functions_map _functions;
    LiteralValue* _returnValue;
};

}

#endif // CONTEXT_H
