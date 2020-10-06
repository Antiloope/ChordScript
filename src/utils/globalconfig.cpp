#include "globalconfig.h"
#include "mINI.h"
#include "log.h"

using namespace CS;
using namespace std;
using namespace mINI;
using namespace ConfigDefinitions;

#define uCast static_cast<unsigned>

const string GLOBAL_CONFIG_INI_FILE = ":/etc/config.ini";
const string ENGLISH_INI_FILE = ":/etc/lang/en.ini";
const string SPANISH_INI_FILE = ":/etc/lang/es.ini";

const string STARTUP_SETTINGS_SECTION = "global";

const string CURRENT_LANGUAGE_KEY = "lang";
const string IS_FIRST_TIME_OPENED_KEY = "firstTime";

enum class ConfigFile {
    GlobalConfig = 0,
    EnglishFile,
    SpanishFile,
    ConfigFileCount
};

GlobalConfig* GlobalConfig::_instance = new GlobalConfig();

GlobalConfig* GlobalConfig::getInstance() {
    return _instance;
}

GlobalConfig::~GlobalConfig() {
    for( unsigned i = 0; i < uCast(ConfigFile::ConfigFileCount); i++ )
    {
        delete _filesData[i];
    }
    delete[] _filesData;
}

GlobalConfig::GlobalConfig() {

    _filesData = new INIStructure*[uCast(ConfigFile::ConfigFileCount)]();
    _filesData[uCast(ConfigFile::GlobalConfig)] = new INIStructure;
    _filesData[uCast(ConfigFile::EnglishFile)] = new INIStructure;
    _filesData[uCast(ConfigFile::SpanishFile)] = new INIStructure;

    INIFile file(GLOBAL_CONFIG_INI_FILE);
    file.read(*_filesData[uCast(ConfigFile::GlobalConfig)]);

    if( !checkIntegrity(_filesData[uCast(ConfigFile::GlobalConfig)]) )
    {
        Log::getInstance().write("Config file is corrupted.",Log::error_t);
        return;
    }

    string lang = (*_filesData[uCast(ConfigFile::GlobalConfig)])
        [STARTUP_SETTINGS_SECTION]
        [CURRENT_LANGUAGE_KEY];

    if( lang == "en" )
        _currentLanguage = Language::English;
    else if( lang == "es" )
        _currentLanguage = Language::Spanish;
    else
    {
        Log::getInstance().write("Invalid language in config file. Default: english",Log::error_t);
        _currentLanguage = Language::English;
    }
}

bool GlobalConfig::checkIntegrity(INIStructure* structure) {
    if( !(*structure).has(STARTUP_SETTINGS_SECTION) )
        return false;
    if( !(*structure)[STARTUP_SETTINGS_SECTION].has(CURRENT_LANGUAGE_KEY) )
        return false;
    if( !(*structure)[STARTUP_SETTINGS_SECTION].has(IS_FIRST_TIME_OPENED_KEY) )
        return false;
    return true;
}

bool GlobalConfig::isFirstTimeOpened() {
    string isFirstTimeOpened = (*_filesData[uCast(ConfigFile::GlobalConfig)])
        [STARTUP_SETTINGS_SECTION]
        [IS_FIRST_TIME_OPENED_KEY];

    if( isFirstTimeOpened == "true" )
        return true;
    return false;
}

bool GlobalConfig::setParameter(
    Section section,
    Parameter parameter,
    string value) {

    switch( section )
    {
    case Section::StartupSettings:
        switch( parameter )
        {
        case Parameter::CurrentLanguage:
            (*_filesData[uCast(ConfigFile::GlobalConfig)])
                [STARTUP_SETTINGS_SECTION]
                [CURRENT_LANGUAGE_KEY] = value;
            break;

        case Parameter::IsFirstTimeOpened:
            (*_filesData[uCast(ConfigFile::GlobalConfig)])
                [STARTUP_SETTINGS_SECTION]
                [IS_FIRST_TIME_OPENED_KEY] = value;
            break;

        default:
            return false;
        }
        break;
    }
    return true;
}

string GlobalConfig::getParameter(
    Section section,
    Parameter parameter) {

    switch( section )
    {
    case Section::StartupSettings:
        switch( parameter )
        {
        case Parameter::CurrentLanguage:
            return (*_filesData[uCast(ConfigFile::GlobalConfig)])
                [STARTUP_SETTINGS_SECTION]
                [CURRENT_LANGUAGE_KEY];
            break;

        case Parameter::IsFirstTimeOpened:
            return (*_filesData[uCast(ConfigFile::GlobalConfig)])
                [STARTUP_SETTINGS_SECTION]
                [IS_FIRST_TIME_OPENED_KEY];
            break;

        default:
            break;
        }
        break;
    }
    return "";
}

void GlobalConfig::setLanguage(Language lang) {
    switch( lang )
    {
    case Language::English:
        (*_filesData[uCast(ConfigFile::GlobalConfig)])
            [STARTUP_SETTINGS_SECTION]
            [CURRENT_LANGUAGE_KEY] = "en";
        _currentLanguage = Language::English;
        break;
    case Language::Spanish:
        (*_filesData[uCast(ConfigFile::GlobalConfig)])
            [STARTUP_SETTINGS_SECTION]
            [CURRENT_LANGUAGE_KEY] = "es";
        _currentLanguage = Language::Spanish;
        break;
    }
}

Language GlobalConfig::getLanguage() {
    return _currentLanguage;
}
