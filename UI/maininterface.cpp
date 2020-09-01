#include "maininterface.h"
#include "ui_mainwindow.h"
#include "codeeditor.h"
#include "interpteter/interpreter.h"
#include "utils/Exceptions/exception.h"

MainInterface::MainInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _editor = new CodeEditor(ui->centralwidget);
    ui->horizontalLayout->addWidget(_editor);

    connect(ui->actionPlay,SIGNAL(triggered()),this,SLOT(playButton()));
}

MainInterface::~MainInterface()
{
    delete ui;
}

void MainInterface::playButton() {
    CS::Interpreter interpreter;

    string code = _editor->toPlainText().replace(QRegExp("[ \t]")," ").toStdString();
    try {
        interpreter.interpret(code);
    } catch (CS::SyntaxException& e) {
        CS::Log::getInstance().write(e.what() + to_string(e.getCharacterRefference()) + code[e.getCharacterRefference()-1] + code[e.getCharacterRefference()] + code[e.getCharacterRefference()+1],CS::Log::info_t);
    } catch (CS::SemanticException& e) {
        CS::Log::getInstance().write(e.what() + to_string(e.getCharacterRefference()) + code[e.getCharacterRefference()-1] + code[e.getCharacterRefference()] + code[e.getCharacterRefference()+1],CS::Log::info_t);
    } catch (exception& e) {
        CS::Log::getInstance().write(e.what(),CS::Log::info_t);
    }

}
