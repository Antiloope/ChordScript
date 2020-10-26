#ifndef GLOBALCONFIGDEFINITIONS_H
#define GLOBALCONFIGDEFINITIONS_H

namespace CS {
namespace ConfigDefinitions {

enum Language
{
    English,
    Spanish,
};

enum Section
{
    StartupSettings,
    TopBar,
};

enum Parameter
{
    IsFirstTimeOpened,
    CurrentLanguage,
    FileTitle,
    Version,
};

}
}

#endif // GLOBALCONFIGDEFINITIONS_H
