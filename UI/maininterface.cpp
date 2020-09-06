#include "maininterface.h"
#include "editor/codeeditor.h"
#include "toolbox/toolbox.h"
#include "consoletabs/consoletabs.h"
#include "utils/Exceptions/exception.h"
#include "uimanager.h"
#include "uidefinitions.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QTextBrowser>
#include <QSplitter>

using namespace CS::UI;

namespace {

const QString WINDOW_NAME = QString::fromUtf8("MainWindow");
const int DEFAULT_WINDOW_WIDTH = 500;
const int DEFAULT_WINDOW_HEIGHT = 800;
const QString WINDOW_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/circulo.svg");
const QString ACTION_PLAY_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/play.svg");
const QString ACTION_STOP_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/stop.svg");
const QString ACTION_RECORD_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/record.svg");
const QString ACTION_RECORDING_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/recording.svg");
const QString ACTION_RESTART_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/restart.svg");
const int DEFAULT_MENU_BAR_HEIGHT = 20;
const QString FILE_MENU_TITLE = QString::fromUtf8("File");
const QString EDIT_MENU_TITLE = QString::fromUtf8("Edit");
const QString VIEW_MENU_TITLE = QString::fromUtf8("View");
const QString SERVER_MENU_TITLE = QString::fromUtf8("Server");
const QString FIND_MENU_TITLE = QString::fromUtf8("Find");
const QString HELP_MENU_TITLE = QString::fromUtf8("Help");

}

MainInterface::MainInterface(UiManager* manager,QWidget *parent)
    : QMainWindow(parent), _manager(manager) {

    UiDefinitions* def = UiDefinitions::getInstance();

    // Setting name, size, color palette and font
    this->setObjectName(WINDOW_NAME);
    this->resize(DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_WIDTH);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);

    this->setPalette( def->getPalette(PaletteId::Global) );
    this->setFont( def->getFont(FontId::Global) );

    // Setting icons on actions
    QIcon icon;
    icon.addFile(WINDOW_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    QAction* actionPlay = new QAction(this);
    QIcon icon1;
    icon1.addFile(ACTION_PLAY_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::Off);
    actionPlay->setObjectName("playButton");
    actionPlay->setIcon(icon1);
    actionPlay->setIconVisibleInMenu(true);
    actionPlay->setShortcutVisibleInContextMenu(true);
    actionPlay->setShortcut(QString("Ctrl+R"));

    QAction* actionStop = new QAction(this);
    QIcon icon2;
    icon2.addFile(ACTION_STOP_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::Off);
    actionStop->setIcon(icon2);

    QAction* actionRecord = new QAction(this);
    actionRecord->setCheckable(true);
    QIcon icon3;
    icon3.addFile(ACTION_RECORD_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::Off);
    icon3.addFile(ACTION_RECORDING_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::On);
    actionRecord->setIcon(icon3);

    QAction* actionRestart = new QAction(this);
    QIcon icon4;
    icon4.addFile(ACTION_RESTART_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::Off);
    actionRestart->setIcon(icon4);

    // Central widget
    QWidget* centralWidget = new QWidget(this);
    sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    centralWidget->setSizePolicy(sizePolicy);
    this->setCentralWidget(centralWidget);

    // Top menu
    QMenuBar* menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_MENU_BAR_HEIGHT));
    menuBar->setAutoFillBackground(false);
    menuBar->setStyleSheet("QMenuBar{background-color:white;}");

    QMenu* menuFile = new QMenu(menuBar);
    menuFile->setTitle(FILE_MENU_TITLE);
    QMenu* menuEdit = new QMenu(menuBar);
    menuEdit->setTitle(EDIT_MENU_TITLE);
    QMenu* menuView = new QMenu(menuBar);
    menuView->setTitle(VIEW_MENU_TITLE);
    QMenu* menuServer = new QMenu(menuBar);
    menuServer->setTitle(SERVER_MENU_TITLE);
    QMenu* menuHelp = new QMenu(menuBar);
    menuHelp->setTitle(HELP_MENU_TITLE);
    QMenu* menuFind = new QMenu(menuBar);
    menuFind->setTitle(FIND_MENU_TITLE);
    this->setMenuBar(menuBar);

    // Status bar
    QStatusBar* statusbar = new QStatusBar(this);
    this->setStatusBar(statusbar);

    // Tool bar
    QToolBar* toolBar = new QToolBar(this);
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(toolBar->sizePolicy().hasHeightForWidth());
    toolBar->setSizePolicy(sizePolicy1);
    toolBar->setMinimumSize(QSize(0, 33));
    toolBar->setAutoFillBackground(true);
    this->addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar->setStyleSheet("QToolBar{background-color:white;} QToolButton{border:none;padding:3px}QToolButton:hover{border-radius:6px;background-color:#1a1a1aff}");

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuEdit->menuAction());
    menuBar->addAction(menuView->menuAction());
    menuBar->addAction(menuFind->menuAction());
    menuBar->addAction(menuServer->menuAction());
    menuBar->addAction(menuHelp->menuAction());

    toolBar->addAction(actionPlay);
    toolBar->addAction(actionStop);
    toolBar->addSeparator();
    toolBar->addAction(actionRecord);
    toolBar->addAction(actionRestart);

    // Creating layout divition
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    centralWidget->setLayout(horizontalLayout);

    // Adding toolbox in the left side
    ToolBox* toolBox = new ToolBox();
    horizontalLayout->addWidget(toolBox);

    // Adding a frame in the right side
    QFrame* editorFrame = new QFrame();
    horizontalLayout->addWidget(editorFrame);

    // Set a vertical layout in the right side frame
    QVBoxLayout* verticalLayout = new QVBoxLayout(editorFrame);
    QSplitter* splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);
    splitter->setChildrenCollapsible(false);
    splitter->setStyleSheet("QSplitter::handle:vertical{height:6px;}");
    verticalLayout->addWidget(splitter);

    // Insert the code editor in the to top side
    _editor = new CodeEditor(splitter);
    splitter->addWidget(_editor);

    // Insert consoles in the bottom side
    _consoleTabs = new ConsoleTabs(splitter);
    splitter->addWidget(_consoleTabs);

    connect(actionPlay,SIGNAL(triggered()),this,SLOT(playButton()));
}

MainInterface::~MainInterface() {
}

#include "interpteter/interpreter.h"

void MainInterface::playButton() {
    CS::Interpreter interpreter;

    string code = _editor->getText().replace(QRegExp("[ \t]")," ").toStdString();
    try {
        interpreter.interpret(code);
    } catch (CS::SyntaxException& e) {
        string log = e.what();
        log += ". At character: " + to_string(e.getCharacterRefference()) + ". Close to: ";
        log += e.getCharacterRefference()==0?"":&code[e.getCharacterRefference()-1];
        log += code[e.getCharacterRefference()];
        log += e.getCharacterRefference()>=code.size()-1?"":&code[e.getCharacterRefference()+1];
        CS::Log::getInstance().write(log,CS::Log::info_t);
    } catch (CS::SemanticException& e) {
        string log = e.what();
        log += ". At character: " + to_string(e.getCharacterRefference()) + ". Close to: ";
        log += e.getCharacterRefference()==0?"":&code[e.getCharacterRefference()-1];
        log += code[e.getCharacterRefference()];
        log += e.getCharacterRefference()>=code.size()-1?"":&code[e.getCharacterRefference()+1];
        CS::Log::getInstance().write(log,CS::Log::info_t);
    } catch (exception& e) {
        CS::Log::getInstance().write(e.what(),CS::Log::info_t);
    }

    ((QTextBrowser*)_consoleTabs->widget(0))->reload();
}
