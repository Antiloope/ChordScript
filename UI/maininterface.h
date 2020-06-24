#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainInterface : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainInterface(QWidget *parent = nullptr);
    ~MainInterface();
private:
    Ui::MainWindow *ui;
signals:

};

#endif // MAININTERFACE_H
