#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QMainWindow>

namespace CS {
namespace UI {

class CodeEditor;
class UiManager;
class ConsoleTabs;

class MainInterface : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainInterface(UiManager*,QWidget *parent = nullptr);
    ~MainInterface();
private:
    UiManager* _manager;
    CodeEditor* _editor;
    ConsoleTabs* _consoleTabs;
protected slots:
    void playButton();
signals:

};

}
}

#endif // MAININTERFACE_H
