#include "globalconfig.h"
#include "mINI.h"
#include "log.h"

using namespace CS;
using namespace std;
using namespace mINI;
using namespace ConfigDefinitions;

const char* GLOBAL_CONFIG_INI_FILE = "./etc/config.ini";
const char* ENGLISH_INI_FILE = "./etc/lang/en.ini";
const char* SPANISH_INI_FILE = "./etc/lang/es.ini";

const char* STARTUP_SETTINGS_SECTION = "global";
const char* TOP_BAR_SECTION = "topBar";

const char* CURRENT_LANGUAGE_KEY = "lang";
const char* IS_FIRST_TIME_OPENED_KEY = "firstTime";
const char* VERSION_KEY = "version";
const char* FILE_TITLE_KEY = "fileTitle";

GlobalConfig* GlobalConfig::_instance = new GlobalConfig();

GlobalConfig* GlobalConfig::getInstance() {
    return _instance;
}

GlobalConfig::~GlobalConfig() {
    for( unsigned i = 0; i < uCast(ConfigFile::ConfigFileCount); i++ )
        delete _filesData[i];
}

GlobalConfig::GlobalConfig() {

    _filesData[uCast(ConfigFile::GlobalConfig)] = new INIStructure;
    _filesData[uCast(ConfigFile::EnglishFile)] = new INIStructure;
    _filesData[uCast(ConfigFile::SpanishFile)] = new INIStructure;

    INIFile fileGlobalConfig(GLOBAL_CONFIG_INI_FILE);
    fileGlobalConfig.read(*_filesData[uCast(ConfigFile::GlobalConfig)]);
    INIFile fileEnglishLanguage(ENGLISH_INI_FILE);
    fileEnglishLanguage.read(*_filesData[uCast(ConfigFile::EnglishFile)]);
    INIFile fileSpanishLanguage(SPANISH_INI_FILE);
    fileSpanishLanguage.read(*_filesData[uCast(ConfigFile::SpanishFile)]);

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
    const char* value) {

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
    default:
        return false;
    }
    return true;
}

const char* GlobalConfig::getParameter(
    Section section,
    Parameter parameter) {

    ConfigFile configFile;
    switch( _currentLanguage )
    {
    case Language::English:
        configFile = ConfigFile::EnglishFile;
        break;
    case Language::Spanish:
        configFile = ConfigFile::SpanishFile;
        break;
    }

    switch( section )
    {
    case Section::StartupSettings:
        switch( parameter )
        {
        case Parameter::CurrentLanguage:
            return (*_filesData[uCast(ConfigFile::GlobalConfig)])
                [STARTUP_SETTINGS_SECTION]
                [CURRENT_LANGUAGE_KEY].c_str();
            break;

        case Parameter::IsFirstTimeOpened:
            return (*_filesData[uCast(ConfigFile::GlobalConfig)])
                [STARTUP_SETTINGS_SECTION]
                [IS_FIRST_TIME_OPENED_KEY].c_str();
            break;

        case Parameter::Version:
            return (*_filesData[uCast(ConfigFile::GlobalConfig)])
                [STARTUP_SETTINGS_SECTION]
                [VERSION_KEY].c_str();
            break;

        default:
            break;
        }
        break;
    case Section::TopBar:
        switch( parameter )
        {
        case Parameter::FileTitle:
            return (*_filesData[uCast(configFile)])
                [TOP_BAR_SECTION]
                [FILE_TITLE_KEY].c_str();
            break;
        default:
            break;
        }
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
