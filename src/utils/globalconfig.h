#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <string>
#include "globalconfigdefinitions.h"

namespace mINI {
template<typename T>
class INIMap;
using INIStructure = INIMap<INIMap<std::string>>;
}

namespace CS {

class GlobalConfig
{
public:
    static GlobalConfig* getInstance();

    bool isFirstTimeOpened();

    bool setParameter(ConfigDefinitions::Section, ConfigDefinitions::Parameter, std::string);
    std::string getParameter(ConfigDefinitions::Section, ConfigDefinitions::Parameter);

    void setLanguage(ConfigDefinitions::Language);
    ConfigDefinitions::Language getLanguage();
    ~GlobalConfig();
private:
    static GlobalConfig* _instance;
    GlobalConfig();

    bool checkIntegrity(mINI::INIStructure*);
    ConfigDefinitions::Language _currentLanguage;
    mINI::INIStructure** _filesData; 
};

}

#endif // GLOBALCONFIG_H
