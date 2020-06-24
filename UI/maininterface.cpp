#include "maininterface.h"
#include "ui_mainwindow.h"

MainInterface::MainInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainInterface::~MainInterface()
{
    delete ui;
}
