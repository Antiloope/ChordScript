#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QMainWindow>

namespace CS {

class Interpreter;

namespace UI {

class CodeEditorTabs;
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
    CodeEditorTabs* _editorTabs;
    ConsoleTabs* _consoleTabs;
    Interpreter* _interpreter;
protected slots:
    void playButton();
    void stopButton();
    void recordButton(bool);
    void openFile();
    void saveFile(int index = -1);
    void saveAsFile(int index = -1);
    void newFile();
    void closeFile(int index = -1);
    void exit();
signals:

};

}
}

#endif // MAININTERFACE_H
