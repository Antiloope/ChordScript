#include "uimanager.h"

uiManager::uiManager()
{
    mainWindow = new MainInterface();
}

uiManager::~uiManager()
{
    delete mainWindow;
}

bool uiManager::init()
{
    mainWindow->show();
    return true;
}
