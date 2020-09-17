#include "maininterface.h"
#include "editor/codeeditor.h"
#include "toolbox/toolbox.h"
#include "consoletabs/consoletabs.h"
#include "utils/Exceptions/exception.h"
#include "uimanager.h"
#include "uidefinitions.h"
#include <iostream>
#include <thread>
#include <mutex>

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
const QString WINDOW_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/circulo.svg");
const QString ACTION_PLAY_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/play.svg");
const QString ACTION_STOP_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/stop.svg");
const QString ACTION_RECORD_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/record.svg");
const QString ACTION_RECORDING_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/recording.svg");
const QString ACTION_RESTART_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/restart.svg");
const int DEFAULT_MENU_BAR_HEIGHT = 20;
const QString FILE_MENU_TITLE = QString::fromUtf8("&File");
const QString EDIT_MENU_TITLE = QString::fromUtf8("&Edit");
const QString VIEW_MENU_TITLE = QString::fromUtf8("&View");
const QString SERVER_MENU_TITLE = QString::fromUtf8("&Server");
const QString FIND_MENU_TITLE = QString::fromUtf8("Fi&nd");
const QString HELP_MENU_TITLE = QString::fromUtf8("&Help");
const QString OPEN_ACTION_TITLE = QString::fromUtf8("&Open...");
const QString OPEN_RECENT_ACTION_TITLE = QString::fromUtf8("Recent &Files");
const QString SAVE_ACTION_TITLE = QString::fromUtf8("&Save");
const QString SAVE_AS_ACTION_TITLE = QString::fromUtf8("Save As...");
const QString CLOSE_FILE_ACTION_TITLE = QString::fromUtf8("&Close");
const QString EXIT_ACTION_TITLE = QString::fromUtf8("E&xit");
const QString UNDO_ACTION_TITLE = QString::fromUtf8("&Undo");
const QString REDO_ACTION_TITLE = QString::fromUtf8("&Redo");
const QString COPY_ACTION_TITLE = QString::fromUtf8("&Copy");
const QString PASTE_ACTION_TITLE = QString::fromUtf8("&Paste");
const QString CUT_ACTION_TITLE = QString::fromUtf8("Cu&t");
const QString ZOOM_IN_ACTION_TITLE = QString::fromUtf8("Zoom In");
const QString ZOOM_OUT_ACTION_TITLE = QString::fromUtf8("Zoom Out");
const QString FIND_ACTION_TITLE = QString::fromUtf8("&Find...");
const QString COMMENT_ACTION_TITLE = QString::fromUtf8("&Toggle Comment");
const QString SERVER_CONFIG_ACTION_TITLE = QString::fromUtf8("Server &Config");
const QString SERVER_KILL_ACTION_TITLE = QString::fromUtf8("&Kill Server");
const QString SERVER_START_ACTION_TITLE = QString::fromUtf8("&Start Server");
const QString SERVER_RESTART_ACTION_TITLE = QString::fromUtf8("&Restart Server");
const QString SERVER_RECORD_ACTION_TITLE = QString::fromUtf8("Recording");

}

atomic_flag flag = ATOMIC_FLAG_INIT;

MainInterface::MainInterface(UiManager* manager,QWidget *parent)
    : QMainWindow(parent), _manager(manager) {

    UiDefinitions* def = UiDefinitions::getInstance();

    // Setting name, size, color palette and font
    this->setObjectName(WINDOW_NAME);
    this->setWindowState(Qt::WindowMaximized);

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
    actionPlay->setIcon(icon1);
    actionPlay->setShortcutVisibleInContextMenu(true);
    actionPlay->setShortcut(QString("Ctrl+Space"));

    QAction* actionStop = new QAction(this);
    QIcon icon2;
    icon2.addFile(ACTION_STOP_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::Off);
    actionStop->setIcon(icon2);
    actionStop->setShortcutVisibleInContextMenu(true);
    actionStop->setShortcut(QString("Ctrl+P"));

    QAction* actionRecord = new QAction(this);
    actionRecord->setCheckable(true);
    QIcon icon3;
    icon3.addFile(ACTION_RECORD_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::On);
    icon3.addFile(ACTION_RECORDING_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::On);
    actionRecord->setIcon(icon3);
    actionRecord->setShortcutVisibleInContextMenu(true);
    actionRecord->setShortcut(QString("Ctrl+R"));

    QAction* actionRestart = new QAction(this);
    QIcon icon4;
    icon4.addFile(ACTION_RESTART_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::Off);
    actionRestart->setIcon(icon4);
    actionRestart->setShortcutVisibleInContextMenu(true);
    actionRestart->setShortcut(QString("Ctrl+T"));

    // Central widget
    QWidget* centralWidget = new QWidget(this);
    sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    centralWidget->setSizePolicy(sizePolicy);
    this->setCentralWidget(centralWidget);

    // Top menu
    QMenuBar* menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_MENU_BAR_HEIGHT));
    menuBar->setAutoFillBackground(false);
    menuBar->setPalette(UiDefinitions::getInstance()->getPalette(PaletteId::Global));
    menuBar->setStyleSheet(
        "QMenuBar{"
            "background-color:" + UiDefinitions::getInstance()->getColorRGB(ColorId::Background) + ";"
        "}"
        "QMenu::item:selected { "
            "background: " + def->getColorRGB(ColorId::Primary) + ";"
        "}"
        );

    QAction* openAction = new QAction(OPEN_ACTION_TITLE,this);
    openAction->setShortcut(tr("CTRL+O"));
    QAction* openRecentAction = new QAction(OPEN_RECENT_ACTION_TITLE,this);
    QAction* saveAction = new QAction(SAVE_ACTION_TITLE,this);
    saveAction->setShortcut(tr("CTRL+S"));
    QAction* saveAsAction = new QAction(SAVE_AS_ACTION_TITLE,this);
    QAction* closeFileAction = new QAction(CLOSE_FILE_ACTION_TITLE,this);
    closeFileAction->setShortcut(tr("CTRL+W"));
    QAction* exitAction = new QAction(EXIT_ACTION_TITLE,this);
    exitAction->setShortcut(tr("CTRL+Q"));
    QAction* undoAction = new QAction(UNDO_ACTION_TITLE,this);
    undoAction->setShortcut(tr("CTRL+Z"));
    QAction* redoAction = new QAction(REDO_ACTION_TITLE,this);
    redoAction->setShortcut(tr("CTRL+SHIFT+Z"));
    QAction* copyAction = new QAction(COPY_ACTION_TITLE,this);
    copyAction->setShortcut(tr("CTRL+C"));
    QAction* cutAction = new QAction(CUT_ACTION_TITLE,this);
    cutAction->setShortcut(tr("CTRL+X"));
    QAction* pasteAction = new QAction(PASTE_ACTION_TITLE,this);
    pasteAction->setShortcut(tr("CTRL+V"));
    QAction* zoomInAction = new QAction(ZOOM_IN_ACTION_TITLE,this);
    zoomInAction->setShortcut(tr("CTRL++"));
    QAction* zoomOutAction = new QAction(ZOOM_OUT_ACTION_TITLE,this);
    zoomOutAction->setShortcut(tr("CTRL+-"));
    QAction* findAction = new QAction(FIND_ACTION_TITLE,this);
    findAction->setShortcut(tr("CTRL+F"));
    QAction* commentAction = new QAction(COMMENT_ACTION_TITLE,this);
    commentAction->setShortcut(tr("CTRL+/"));
    QAction* serverConfigAction = new QAction(SERVER_CONFIG_ACTION_TITLE,this);
    QAction* serverKillAction = new QAction(SERVER_KILL_ACTION_TITLE,this);
    QAction* serverStartAction = new QAction(SERVER_START_ACTION_TITLE,this);
    QAction* serverRestartAction = new QAction(SERVER_RESTART_ACTION_TITLE,this);
    serverRestartAction->setShortcut(tr("CTRL+T"));
    QAction* serverRecordAction = new QAction(SERVER_RECORD_ACTION_TITLE,this);
    serverRecordAction->setCheckable(true);
    serverRecordAction->setShortcut(tr("CTRL+R"));

    QMenu* menuFile = new QMenu(menuBar);
    menuFile->setTitle(FILE_MENU_TITLE);
    menuFile->addAction(openAction);
    menuFile->addAction(openRecentAction);
    menuFile->addSeparator();
    menuFile->addAction(saveAction);
    menuFile->addAction(saveAsAction);
    menuFile->addSeparator();
    menuFile->addAction(closeFileAction);
    menuFile->addAction(exitAction);
    QMenu* menuEdit = new QMenu(menuBar);
    menuEdit->setTitle(EDIT_MENU_TITLE);
    menuEdit->addAction(undoAction);
    menuEdit->addAction(redoAction);
    menuEdit->addSeparator();
    menuEdit->addAction(copyAction);
    menuEdit->addAction(cutAction);
    menuEdit->addAction(pasteAction);
    menuEdit->addSeparator();
    menuEdit->addAction(zoomInAction);
    menuEdit->addAction(zoomOutAction);
    menuEdit->addSeparator();
    menuEdit->addAction(findAction);
    menuEdit->addAction(commentAction);
    QMenu* menuView = new QMenu(menuBar);
    menuView->setTitle(VIEW_MENU_TITLE);
    QMenu* menuServer = new QMenu(menuBar);
    menuServer->setTitle(SERVER_MENU_TITLE);
    menuServer->addAction(serverStartAction);
    menuServer->addAction(serverKillAction);
    menuServer->addAction(serverRestartAction);
    menuServer->addSeparator();
    menuServer->addAction(serverRecordAction);
    menuServer->addSeparator();
    menuServer->addAction(serverConfigAction);
    QMenu* menuHelp = new QMenu(menuBar);
    menuHelp->setTitle(HELP_MENU_TITLE);
    QMenu* menuFind = new QMenu(menuBar);
    menuFind->setTitle(FIND_MENU_TITLE);
    this->setMenuBar(menuBar);

    // Status bar
    QStatusBar* statusbar = new QStatusBar(this);
    this->setStatusBar(statusbar);

    // Tool bar
    QToolBar* toolBar = new QToolBar(centralWidget);
    toolBar->setOrientation(Qt::Vertical);
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(toolBar->sizePolicy().hasHeightForWidth());
    toolBar->setSizePolicy(sizePolicy1);
    toolBar->setMinimumSize(QSize(0, 33));
    toolBar->setAutoFillBackground(true);
    this->addToolBar(Qt::LeftToolBarArea, toolBar);
    toolBar->setStyleSheet(
        "QToolBar{"
        "background-color:"+UiDefinitions::getInstance()->getColorRGB(ColorId::Background)+";"
            "border:none;"
        "} "
        "QToolButton{"
            "border:none;"
            "padding:3px;"
        "}"
        "QToolButton:hover{"
            "border-radius:6px;"
            "background-color:#1a1a1aff;"
        "}");

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

    QSplitter* hSplitter = new QSplitter();
    hSplitter->setOrientation(Qt::Horizontal);
    hSplitter->setChildrenCollapsible(false);
    hSplitter->setStyleSheet(
        "QSplitter::handle:horizontal{"
            "width:10px;"
        "}");
    horizontalLayout->addWidget(hSplitter);

    // Insert the code editor on the top side
    _editor = new CodeEditor();
    hSplitter->addWidget(_editor);

    // Adding a frame in the left side
    QFrame* toolBoxFrame = new QFrame();
    hSplitter->addWidget(toolBoxFrame);

    toolBoxFrame->setObjectName("toolBoxFrame");

    toolBoxFrame->setStyleSheet(
        "#toolBoxFrame{"
            "border:5px solid " + UiDefinitions::getInstance()->getColorRGB(ColorId::LightGray) + ";"
            "padding: -5px;"
        "}"
    );
    toolBoxFrame->setMaximumWidth(hSplitter->frameSize().width()*0.7);

    // Set a vertical layout in the right side frame
    QVBoxLayout* verticalLayout = new QVBoxLayout();
    toolBoxFrame->setLayout(verticalLayout);

    QSplitter* vSplitter = new QSplitter();
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->setChildrenCollapsible(false);
    vSplitter->setStyleSheet(
        "QSplitter::handle:vertical{"
            "height:-1px;"
        "}"
        "QSplitter::handle:vertical{"
            "padding-top:-1px;"
            "padding-bottom:-1px;"
            "margin-top: 5px;"
            "margin-bottom: 5px;"
            "background: "+ def->getColorRGB(ColorId::LightGray) + ";"
        "}");
    verticalLayout->addWidget(vSplitter);

    // Adding toolbox in the left side
    ToolBox* toolBox = new ToolBox();
    vSplitter->addWidget(toolBox);

    // Insert consoles in the bottom side
    _consoleTabs = new ConsoleTabs(vSplitter);
    vSplitter->addWidget(_consoleTabs);
    vSplitter->setStretchFactor(0,2);

    connect(actionPlay,SIGNAL(triggered()),this,SLOT(playButton()));
    connect(actionStop,SIGNAL(triggered()),this,SLOT(stopButton()));
    connect(openAction,SIGNAL(triggered()),this,SLOT(openFile()));
//    connect(openRecentAction,SIGNAL(triggered()),this,SLOT(openRecentFile()));
//    connect(saveAction,SIGNAL(triggered()),this,SLOT(saveFile()));
//    connect(saveAsAction,SIGNAL(triggered()),this,SLOT(saveAsFile()));
//    connect(closeFileAction,SIGNAL(triggered()),this,SLOT(closeFile()));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(exit()));
//    connect(undoAction,SIGNAL(triggered()),this,SLOT(undo()));
//    connect(redoAction,SIGNAL(triggered()),this,SLOT(redo()));
//    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));
//    connect(cutAction,SIGNAL(triggered()),this,SLOT(cut()));
//    connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));
//    connect(zoomInAction,SIGNAL(triggered()),this,SLOT(zoomIn()));
//    connect(zoomOutAction,SIGNAL(triggered()),this,SLOT(zoomOut()));
//    connect(findAction,SIGNAL(triggered()),this,SLOT(find()));
//    connect(commentAction,SIGNAL(triggered()),this,SLOT(comment()));
//    connect(serverStartAction,SIGNAL(triggered()),this,SLOT(startServer()));
//    connect(serverKillAction,SIGNAL(triggered()),this,SLOT(killServer()));
//    connect(serverRestartAction,SIGNAL(triggered()),this,SLOT(restartServer()));
//    connect(serverRecordAction,SIGNAL(triggered()),this,SLOT(record()));
//    connect(serverConfigAction,SIGNAL(triggered()),this,SLOT(configServer()));
}

MainInterface::~MainInterface() {
}

#include "interpteter/interpreter.h"

void MainInterface::stopButton() {
    _interpreter->interpret("STOP();");
}

void MainInterface::openFile() {
    this->_editor->setPlainText("sound a = S_SIN+S_SIN.constantFreq(1000);\na.play([440],1);\n");
}

void MainInterface::exit() {
    this->close();
}

void MainInterface::playButton() {
    if(!flag.test_and_set())
    {
        string code = _editor->getText().replace(QRegExp("[ \t]")," ").toStdString();
        try {
            _interpreter->interpret(code);
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
        flag.clear();
    }
}
