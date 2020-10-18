#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QMainWindow>
#include "uidefinitions.h"

namespace CS {

class Interpreter;

namespace UI {

class CodeEditorTabs;
class ConsoleTabs;
class TopBar;

class MainInterface : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainInterface(QWidget *parent = nullptr);
    ~MainInterface();
private:
    CodeEditorTabs* _editorTabs;
    ConsoleTabs* _consoleTabs;
    TopBar* _topBar;
protected slots:
    void playButton();
    void stopButton();
    void recordButton(bool);
    void openFile();
    void find();
    void saveFile(int index = -1);
    void saveAsFile(int index = -1);
    void newFile();
    void closeFile(int index = -1);
    void exit();
    void startServer();
    void restartServer();
    void killServer();
signals:

};

}
}

#endif // MAININTERFACE_H
