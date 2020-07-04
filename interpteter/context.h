#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <tuple>

#include "value.h"
#include "datatype.h"

using namespace std;
namespace CS {

class Context
{
public:
    static Context* getInstance();
    bool isDataType(string);
    bool nameExist(string);
    bool isValidName(string);
    string getDataTypeName(string);
    void newVariable(string,LinkedValue*);
private:
    static Context* _instance;
    Context();
    unordered_map<string,DataType*> _dataTypes;
    unordered_set<string> _reservedKeywords;
    unordered_map<string,tuple<LinkedValue*,LiteralValue*>> _variables;
    //unordered_map<string,FunctionDefinition*> _functions;
};

}

#endif // CONTEXT_H
