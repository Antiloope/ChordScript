#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "maininterface.h"

namespace CS {
namespace UI {

class UiManager {
public:
    UiManager();
    ~UiManager();
    bool init(QApplication&);
private:
    MainInterface *mainWindow;
};

}
}

#endif // UIMANAGER_H
