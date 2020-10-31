#include "outputlog.h"
#include "UI/uidefinitions.h"
#include <QScrollBar>

using namespace CS::UI;

OutputLog::OutputLog(QWidget *parent) : QTextBrowser(parent)
{
    setSource(QUrl("output.txt"),QTextDocument::MarkdownResource);
    setStyleSheet(
        "QTextBrowser {"
            "selection-background-color:" + UiDefinitions::getInstance()->getColorRGB(ColorId::Primary) + ";"
        "}");
    setFont(UiDefinitions::getInstance()->getFont(FontId::Code));

    connect( this, SIGNAL( textChanged() ), this, SLOT( scrollToBottom() ) );
}

void OutputLog::scrollToBottom() {
    this->verticalScrollBar()->setValue( this->verticalScrollBar()->maximum() );
}
