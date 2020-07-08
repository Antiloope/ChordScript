#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <tuple>

#include "value.h"
#include "datatype.h"

using namespace std;
namespace CS {

class FunctionDefinition;

typedef unordered_map<string,tuple<string,LinkedValue*,LiteralValue*>> variables_map;
typedef map<size_t,variables_map> context_map;
typedef unordered_map<string,tuple<string,FunctionDefinition*>> functions_map;
typedef unordered_map<string,DataType*> data_types_map;
typedef unordered_set<string> reserved_keywords_map;

const size_t GlobalContext = 0;

class Context
{
public:
    static Context* getInstance();
    bool isDataType(string);
    bool nameExist(string);
    bool functionNameExist(string);
    bool isValidName(string);
    string getDataTypeName(string);
    void newVariable(string,string,LinkedValue*);
    void newFunction(string,string,FunctionDefinition*);

    size_t newContext();
    size_t switchContext(size_t);
    size_t getCurrentContext() const;
private:
    static Context* _instance;
    Context();
    size_t _currentContext;
    data_types_map _dataTypes;
    reserved_keywords_map _reservedKeywords;
    context_map _variables;
    functions_map _functions;
};

}

#endif // CONTEXT_H
