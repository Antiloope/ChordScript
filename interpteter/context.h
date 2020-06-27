#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;
namespace CS {

class Context
{
public:
    static Context* getInstance();
    bool isDataType(string);
    bool nameExist(string);
    bool isValidName(string);
private:
    static Context* _instance;
    Context();
    unordered_map<string,string> _dataTypes;
    unordered_set<string> _reservedKeywords;
    unordered_map<string,string> _variableNames;
};

}

#endif // CONTEXT_H
