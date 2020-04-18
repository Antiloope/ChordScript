#include "uihandler.h"

uiHandler::uiHandler()
{
    mainWindow = new MainWindow();
}

uiHandler::~uiHandler()
{
    delete mainWindow;
}

bool uiHandler::init()
{
    mainWindow->show();
    return true;
}
