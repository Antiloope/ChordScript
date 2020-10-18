#include "consoletabs.h"
#include "UI/uidefinitions.h"

#include <QTextBrowser>
#include <QPushButton>
#include <QTabBar>
#include <QSplitter>
#include "finder.h"
#include "outputlog.h"

using namespace CS::UI;

namespace {
const QString CLOSE_TAB_ICON_RESOURCE = QString::fromUtf8(":/resources/closeTab.svg");
}

ConsoleTabs::ConsoleTabs(QWidget *parent)
    : QTabWidget(parent) {
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

    _outputLog = new OutputLog(this);
    this->addTab(_outputLog,"Output Log");

    _finder = new Finder(this);
    this->addTab(_finder,"Find");

    connect(_finder,SIGNAL(find(bool,bool,bool,QString)),this,SLOT(findRequested(bool,bool,bool,QString)));
}

void ConsoleTabs::findRequested(bool reverse,bool caseSensitive,bool wholeWords,QString textToFind) {
    emit find(reverse,caseSensitive,wholeWords,textToFind);
}

void ConsoleTabs::setFocusFind() {
    setCurrentIndex(1);
    _finder->setFocusFind();
}
