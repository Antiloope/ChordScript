#include "uimanager.h"
#include <QApplication>
#include "uidefinitions.h"

using namespace CS::UI;

UiManager::UiManager() {
    mainWindow = new MainInterface(this);
}

UiManager::~UiManager() {
    delete mainWindow;
}

bool UiManager::init(QApplication& app) {
    app.setWindowIcon(UiDefinitions::getInstance()->getIcon(IconId::App));
    app.setStyleSheet("QPushButton{background-color:#ffffffff}");
    mainWindow->show();
    return true;
}
