#include "maininterface.h"
#include "editor/codeeditortabs.h"
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
#include <QMessageBox>
#include <QFileDialog>

using namespace CS::UI;

namespace {

const QString WINDOW_NAME = QString::fromUtf8("MainWindow");
const int DEFAULT_WINDOW_WIDTH = 500;
const QString WINDOW_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/circulo.svg");
const QString ACTION_PLAY_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/play.svg");
const QString ACTION_STOP_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/stop.svg");
const QString ACTION_RECORD_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/record.svg");
const QString ACTION_RECORDING_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/recording.svg");
const QString ACTION_SAVE_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/save.svg");
const QString ACTION_NEW_FILE_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/new.svg");
const QString ACTION_OPEN_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/open.svg");
const int DEFAULT_MENU_BAR_HEIGHT = 20;
const QString FILE_MENU_TITLE = QString::fromUtf8("&File");
const QString EDIT_MENU_TITLE = QString::fromUtf8("&Edit");
const QString SERVER_MENU_TITLE = QString::fromUtf8("&Server");
const QString HELP_MENU_TITLE = QString::fromUtf8("&Help");
const QString OPEN_ACTION_TITLE = QString::fromUtf8("&Open...");
const QString NEW_FILE_ACTION_TITLE = QString::fromUtf8("&New File");
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
const QString TUTORIAL_ACTION_TITLE = QString::fromUtf8("&Tutorial");
const QString ABOUT_ACTION_TITLE = QString::fromUtf8("&About ChordScript");
const QString CHECK_UPDATES_ACTION_TITLE = QString::fromUtf8("Check for &Updates");
const QString REPORT_BUG_ACTION_TITLE = QString::fromUtf8("&Report Bug");
const QString DOCUMENTATION_ACTION_TITLE = QString::fromUtf8("Open &Documentation Website");

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
    actionPlay->setToolTip("Run");

    QAction* actionStop = new QAction(this);
    QIcon icon2;
    icon2.addFile(ACTION_STOP_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::Off);
    actionStop->setIcon(icon2);
    actionStop->setShortcutVisibleInContextMenu(true);
    actionStop->setShortcut(QString("Ctrl+P"));
    actionStop->setToolTip("Stop All");

    QAction* actionRecord = new QAction(this);
    actionRecord->setCheckable(true);
    QIcon icon3;
    icon3.addFile(ACTION_RECORD_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::On);
    icon3.addFile(ACTION_RECORDING_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::On);
    actionRecord->setIcon(icon3);
    actionRecord->setShortcutVisibleInContextMenu(true);
    actionRecord->setToolTip("Start/Stop Recording");

    QAction* actionSave = new QAction(this);
    QIcon icon4;
    icon4.addFile(ACTION_SAVE_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::On);
    actionSave->setIcon(icon4);
    actionSave->setShortcutVisibleInContextMenu(true);
    actionSave->setToolTip("Save File");

    QAction* actionNewFile = new QAction(this);
    QIcon icon5;
    icon5.addFile(ACTION_NEW_FILE_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::On);
    actionNewFile->setIcon(icon5);
    actionNewFile->setShortcutVisibleInContextMenu(true);
    actionNewFile->setToolTip("New File");

    QAction* actionOpen= new QAction(this);
    QIcon icon6;
    icon6.addFile(ACTION_OPEN_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::On);
    actionOpen->setIcon(icon6);
    actionOpen->setShortcutVisibleInContextMenu(true);
    actionOpen->setToolTip("Open File");

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
    QAction* newFileAction = new QAction(NEW_FILE_ACTION_TITLE,this);
    newFileAction->setShortcut(tr("CTRL+T"));
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
    QAction* serverRecordAction = new QAction(SERVER_RECORD_ACTION_TITLE,this);
    serverRecordAction->setCheckable(true);
    serverRecordAction->setShortcut(tr("CTRL+R"));
    QAction* tutorialAction = new QAction(TUTORIAL_ACTION_TITLE,this);
    QAction* aboutAction = new QAction(ABOUT_ACTION_TITLE,this);
    QAction* checkUpdatesAction = new QAction(CHECK_UPDATES_ACTION_TITLE,this);
    QAction* reportBugAction = new QAction(REPORT_BUG_ACTION_TITLE,this);
    QAction* documentationAction = new QAction(DOCUMENTATION_ACTION_TITLE,this);

    QMenu* menuFile = new QMenu(menuBar);
    menuFile->setTitle(FILE_MENU_TITLE);
    menuFile->addAction(newFileAction);
    menuFile->addSeparator();
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
    menuHelp->addAction(documentationAction);
    menuHelp->addAction(tutorialAction);
    menuHelp->addSeparator();
    menuHelp->addAction(aboutAction);
    menuHelp->addAction(reportBugAction);
    menuHelp->addSeparator();
    menuHelp->addAction(checkUpdatesAction);
    this->setMenuBar(menuBar);

    // Status bar
    QStatusBar* statusbar = new QStatusBar(this);
    this->setStatusBar(statusbar);

    // Tool bar
    QToolBar* toolBar = new QToolBar(centralWidget);
    toolBar->setOrientation(Qt::Vertical);
    toolBar->setMovable(false);
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
            "background-color:" + UiDefinitions::getInstance()->getColorRGB(ColorId::Background)+";"
            "border:none;"
            "margin:5px;"
        "} "
        "QToolButton{"
            "border:none;"
            "padding:3px;"
        "}"
        "QToolButton:hover{"
            "border-radius:6px;"
            "background-color:#1a1a1aff;"
        "}"
        "QToolTip {"
            "border: 1px solid #f7d5fe;"
            "background-color: #ffffff;"
            "padding:3px;"
        "}");

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuEdit->menuAction());
    menuBar->addAction(menuServer->menuAction());
    menuBar->addAction(menuHelp->menuAction());

    toolBar->addAction(actionPlay);
    toolBar->addAction(actionStop);
    toolBar->addSeparator();
    toolBar->addAction(actionRecord);
    toolBar->addSeparator();
    toolBar->addAction(actionNewFile);
    toolBar->addAction(actionOpen);
    toolBar->addAction(actionSave);

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
    _editorTabs = new CodeEditorTabs();
    hSplitter->addWidget(_editorTabs);
    _editorTabs->widget(_editorTabs->currentIndex())->setFocus();

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

    connect(_consoleTabs,SIGNAL(find(bool,bool,bool,QString)),_editorTabs,SLOT(find(bool,bool,bool,QString)));
    connect(actionPlay,SIGNAL(triggered()),this,SLOT(playButton()));
    connect(actionStop,SIGNAL(triggered()),this,SLOT(stopButton()));
    connect(actionRecord,SIGNAL(triggered(bool)),this,SLOT(recordButton(bool)));
    connect(actionRecord,SIGNAL(triggered(bool)),serverRecordAction,SLOT(setChecked(bool)));
    connect(actionOpen,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(openAction,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(actionSave,SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(actionNewFile,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(newFileAction,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(closeFileAction,SIGNAL(triggered()),this,SLOT(closeFile()));
//    connect(openRecentAction,SIGNAL(triggered()),this,SLOT(openRecentFile()));
    connect(saveAction,SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(saveAsAction,SIGNAL(triggered()),this,SLOT(saveAsFile()));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(exit()));
//    connect(undoAction,SIGNAL(triggered()),this,SLOT(undo()));
//    connect(redoAction,SIGNAL(triggered()),this,SLOT(redo()));
//    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));
//    connect(cutAction,SIGNAL(triggered()),this,SLOT(cut()));
//    connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));
//    connect(zoomInAction,SIGNAL(triggered()),this,SLOT(zoomIn()));
//    connect(zoomOutAction,SIGNAL(triggered()),this,SLOT(zoomOut()));
    connect(findAction,SIGNAL(triggered()),this,SLOT(find()));
//    connect(commentAction,SIGNAL(triggered()),this,SLOT(comment()));
//    connect(serverStartAction,SIGNAL(triggered()),this,SLOT(startServer()));
//    connect(serverKillAction,SIGNAL(triggered()),this,SLOT(killServer()));
//    connect(serverRestartAction,SIGNAL(triggered()),this,SLOT(restartServer()));
    connect(serverRecordAction,SIGNAL(triggered(bool)),this,SLOT(recordButton(bool)));
    connect(serverRecordAction,SIGNAL(triggered(bool)),actionRecord,SLOT(setChecked(bool)));
//    connect(serverConfigAction,SIGNAL(triggered()),this,SLOT(configServer()));
    connect(_editorTabs,SIGNAL(tabCloseRequested(int)),this,SLOT(closeFile(int)));
}

MainInterface::~MainInterface() {
}

#include "interpteter/interpreter.h"

void MainInterface::stopButton() {
    _interpreter->interpret("STOP();");
}

void MainInterface::find() {
    _consoleTabs->setFocusFind();
}

void MainInterface::recordButton(bool checked) {
    if( checked )
        _interpreter->interpret("START_RECORDING();");
    else
        _interpreter->interpret("STOP_RECORDING();");
}

void MainInterface::openFile() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "..",
        tr("Source (*.csf);; Text file (*.txt)"));

    if( !fileName.isNull() )
        _editorTabs->openFile(fileName);
}

void MainInterface::closeFile(int index) {
    if( !_editorTabs->closeFile(index) )
    {
        int ret = QMessageBox::question(
            this,
            "Closing file",
            "Do you want to save unsaved changes on " + (index > 0 ? _editorTabs->tabText(index) : _editorTabs->tabText(_editorTabs->currentIndex())) + "?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);

        switch( ret )
        {
        case QMessageBox::Save:
            if( !_editorTabs->saveFile(index) )
                saveAsFile();
            _editorTabs->closeFile(index);
            break;
        case QMessageBox::Discard:
            _editorTabs->closeFile(index,true);
            break;
        default:
            // should never be reached
            break;
        }
    }
}

void MainInterface::newFile() {
    _editorTabs->newFile();
}

void MainInterface::saveAsFile(int index) {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save As"),
        "..",
        tr("Source (*.csf)")
        );

    if( !fileName.isNull() )
    {
        fileName = fileName.contains(".")?fileName:(fileName + ".csf");
        _editorTabs->saveFile(fileName,index);
    }
}

void MainInterface::saveFile(int index) {
    if( !_editorTabs->saveFile(index) )
    {
        QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save As"),
            "..",
            tr("Source (*.csf)")
            );

        if( !fileName.isNull() )
        {
            fileName = fileName.contains(".")?fileName:(fileName + ".csf");
            _editorTabs->saveFile(fileName,index);
        }
    }
}

void MainInterface::exit() {
    this->close();
}

void MainInterface::playButton() {
    if(!flag.test_and_set())
    {
        string code = ((CodeEditor*)_editorTabs->currentWidget())->getText().replace(QRegExp("[ \t]")," ").toStdString();
        try {
            _interpreter->interpret(code);
        } catch (CS::SyntaxException& e) {
            _editorTabs->setError(e.getCharacterRefference());
            CS::Log::getInstance().write(e.what(),CS::Log::info_t);
        } catch (CS::SemanticException& e) {
            _editorTabs->setError(e.getCharacterRefference());
            CS::Log::getInstance().write(e.what(),CS::Log::info_t);
        } catch (exception& e) {
            CS::Log::getInstance().write(e.what(),CS::Log::info_t);
        }

        ((QTextBrowser*)_consoleTabs->widget(0))->reload();
        flag.clear();
    }
}
