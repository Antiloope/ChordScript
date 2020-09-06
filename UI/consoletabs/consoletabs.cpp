#include "consoletabs.h"
#include "UI/uidefinitions.h"

#include <QTextBrowser>
#include <QPushButton>
#include <QTabBar>
#include <QSplitter>

using namespace CS::UI;

namespace {
const QString CLOSE_TAB_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/closeTab.svg");
}

ConsoleTabs::ConsoleTabs(QWidget *parent)
    : QTabWidget(parent) {
    this->setMaximumHeight(((QSplitter*)(parent))->frameSize().height()/2);
    this->setDocumentMode(true);

    QIcon closeTab;
    closeTab.addFile(CLOSE_TAB_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::Off);
    QPushButton* closeButton = new QPushButton(closeTab,"");
    closeButton->setStyleSheet("QPushButton{border:none;margin-bottom:6px;padding:4px;}QPushButton:hover{border-radius:6px;background-color:#1a1a1aff}");
    closeButton->setIconSize(QSize(25,16));
    this->setCornerWidget(closeButton);

    QTextBrowser* log = new QTextBrowser(this);
    log->setSource(QUrl("log.md"),QTextDocument::MarkdownResource);
    log->setFont(UiDefinitions::getInstance()->getFont(FontId::Monospace));
    log->setAcceptRichText(true);
    this->addTab(log,"Output Log");

    QTextBrowser* searchResult = new QTextBrowser(this);
    searchResult->setSource(QUrl("log.md"),QTextDocument::MarkdownResource);
    searchResult->setFont(UiDefinitions::getInstance()->getFont(FontId::Monospace));
    searchResult->setAcceptRichText(true);
    this->addTab(searchResult,"Search Results");

    QTextBrowser* serverLog = new QTextBrowser(this);
    serverLog->setSource(QUrl("log.md"),QTextDocument::MarkdownResource);
    serverLog->setFont(UiDefinitions::getInstance()->getFont(FontId::Monospace));
    serverLog->setAcceptRichText(true);
    this->addTab(serverLog,"Server Log");

    this->connect(closeButton,SIGNAL(clicked()),this,SLOT(minimize()));
    this->connect(this,SIGNAL(tabBarClicked(int)),this,SLOT(maximize()));
}

void ConsoleTabs::minimize() {
    this->setFixedHeight(this->tabBar()->height());
}

void ConsoleTabs::maximize() {
    this->setMaximumHeight(((QSplitter*)(this->parent()))->frameSize().height());
}
