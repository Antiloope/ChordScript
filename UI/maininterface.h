#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CodeEditor;

class MainInterface : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainInterface(QWidget *parent = nullptr);
    ~MainInterface();
private:
    Ui::MainWindow *ui;
    CodeEditor* _editor;
protected slots:
    void playButton();
signals:

};

#endif // MAININTERFACE_H
