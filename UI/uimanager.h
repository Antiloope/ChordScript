#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "maininterface.h"

class uiManager
{
public:
    uiManager();
    ~uiManager();
    bool init();
private:
    MainInterface * mainWindow;
};

#endif // UIMANAGER_H
