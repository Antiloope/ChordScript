#ifndef UIHANDLER_H
#define UIHANDLER_H

#include "mainwindow.h"

class uiHandler
{
public:
    uiHandler();
    ~uiHandler();
    bool init();
private:
    MainWindow * mainWindow;
};

#endif // UIHANDLER_H
