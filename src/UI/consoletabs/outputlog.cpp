#include "outputlog.h"
#include "UI/uidefinitions.h"
#include <QScrollBar>

using namespace CS::UI;

OutputLog::OutputLog(QWidget *parent) : QTextBrowser(parent)
{
    setFixedHeight(100);
    setSource(QUrl("output.txt"),QTextDocument::MarkdownResource);
    setStyleSheet(
        "QTextBrowser {"
            "selection-background-color:" + UiDefinitions::getInstance()->getColorRGB(ColorId::Primary) + ";"
            "font-size: xx-large;"
        "}");
    auto font = UiDefinitions::getInstance()->getFont(FontId::Code);
    font.setPointSize(16);
    setFont(font);

    connect( this, SIGNAL( textChanged() ), this, SLOT( scrollToBottom() ) );
}

void OutputLog::scrollToBottom() {
    this->verticalScrollBar()->setValue( this->verticalScrollBar()->maximum() );
}
