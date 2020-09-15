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
    //this->setDocumentMode(true);
    UiDefinitions* def = UiDefinitions::getInstance();

    this->setFont(def->getFont(FontId::Global));
    this->setStyleSheet(
        "QTextBrowser{"
            "border: none;"
        "}"
        "QTabBar::tab:!selected {"
            "background: "+ def->getColorRGB(ColorId::Light) + ";"
            "color: " + def->getColorRGB(ColorId::TextPrimary) + ";"
            "padding: 7px;"
            "border: 3px solid " + def->getColorRGB(ColorId::Dark) + ";"
        "}"
        "QTabWidget::tab-bar {"
            "alignment: center;"
        "}"
        "QTabWidget::pane {"
            "border-top: 2px solid " + def->getColorRGB(ColorId::Dark) + ";"
            "position: absolute;"
            "top: -0.8em;"
            "padding-top: 0.8em;"
        "}"
        "QTabBar::tab:selected {"
            "padding: 10px;"
            "background: "+ def->getColorRGB(ColorId::Dark) + ";"
            "color: " + def->getColorRGB(ColorId::Lightest) + ";"
        "}"
        );

    QTextBrowser* log = new QTextBrowser(this);
    log->setSource(QUrl("log.md"),QTextDocument::MarkdownResource);
    log->setFont(UiDefinitions::getInstance()->getFont(FontId::Code));
    log->setAcceptRichText(true);
    this->addTab(log,"Output Log");

    QTextBrowser* searchResult = new QTextBrowser(this);
    searchResult->setSource(QUrl("log.md"),QTextDocument::MarkdownResource);
    searchResult->setFont(UiDefinitions::getInstance()->getFont(FontId::Code));
    searchResult->setAcceptRichText(true);

    this->addTab(searchResult,"Search Results");

    QTextBrowser* serverLog = new QTextBrowser(this);
    serverLog->setSource(QUrl("log.md"),QTextDocument::MarkdownResource);
    serverLog->setFont(UiDefinitions::getInstance()->getFont(FontId::Code));
    serverLog->setAcceptRichText(true);
    this->addTab(serverLog,"Server Log");

}
