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

/**
 * @brief Singleton class to manage config file, giving access
 * to different config parameters.
 */
class GlobalConfig
{
public:
    static GlobalConfig* getInstance();

    /**
     * @brief Sets the value of a parameter on INI file.
     * @param parameter Parameter ID.
     * @param value Value to assign on this parameter.
     * @return true if value could be set correctly.
     */
    bool setParameter(ConfigDefinitions::Parameter parameter, const char* value);
    /**
     * @brief Gets the value of a particular parameter on INI file.
     * @param parameter Parameter ID.
     * @return Value of the stored parameter.
     */
    const char* getParameter(ConfigDefinitions::Parameter parameter);

    ~GlobalConfig();
private:
    static GlobalConfig* _instance;
    GlobalConfig();

    /**
     * @brief Verify that the INI file has the right parameters.
     * @return true if INI file has all the needed parameters.
     */
    bool checkIntegrity(mINI::INIStructure*);

    mINI::INIStructure* _fileData;
};

}

#endif // GLOBALCONFIG_H
