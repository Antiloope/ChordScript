#include "globalconfig.h"
#include "mINI.h"
#include "log.h"

using namespace CS;
using namespace std;
using namespace mINI;
using namespace ConfigDefinitions;

const char* GLOBAL_CONFIG_INI_FILE;

const char* GLOBAL_SECTION = "global";
const char* VERSION_KEY = "version";

GlobalConfig* GlobalConfig::_instance = nullptr;

GlobalConfig* GlobalConfig::getInstance() {
    if ( !_instance )
        _instance = new GlobalConfig();
    return _instance;
}

GlobalConfig::~GlobalConfig() {
    delete _fileData;
}

GlobalConfig::GlobalConfig() {
    string auxPath = string(getenv("HOME")) + "/.config/ChordScript/config.ini";
    GLOBAL_CONFIG_INI_FILE = auxPath.c_str();

    _fileData = new INIStructure;

    INIFile fileGlobalConfig(GLOBAL_CONFIG_INI_FILE);
    fileGlobalConfig.read(*_fileData);

    if( !checkIntegrity(_fileData) )
    {
        Log::getInstance().write("Config file is corrupted.",Log::error_t);
        delete _fileData;
        throw new exception;
    }
}

bool GlobalConfig::checkIntegrity(INIStructure* structure) {
    if( !(*structure).has(GLOBAL_SECTION) )
        return false;
    return true;
}

bool GlobalConfig::setParameter(
    Parameter parameter,
    const char* value) {
    try
    {
        switch( parameter )
        {
        case Parameter::Version:
            (*_fileData)
                [GLOBAL_SECTION]
                [VERSION_KEY] = value;
            break;
        default:
            return false;
        }
    }
    catch( ... )
    {
        return false;
    }

    return true;
}

const char* GlobalConfig::getParameter(
    Parameter parameter) {
    try
    {
        switch( parameter )
        {
        case Parameter::Version:
            return (*_fileData)
                [GLOBAL_SECTION]
                [VERSION_KEY].c_str();
        default:
            break;
        }
    }
    catch( ... )
    {}

    return nullptr;
}
