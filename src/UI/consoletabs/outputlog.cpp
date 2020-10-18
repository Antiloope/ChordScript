#include "outputlog.h"
#include "UI/uidefinitions.h"

using namespace CS::UI;

OutputLog::OutputLog(QWidget *parent) : QTextBrowser(parent)
{
    setSource(QUrl("log.md"),QTextDocument::MarkdownResource);
    setStyleSheet(
        "QTextBrowser {"
            "selection-background-color:" + UiDefinitions::getInstance()->getColorRGB(ColorId::Primary) + ";"
        "}");
    setFont(UiDefinitions::getInstance()->getFont(FontId::Code));
    setAcceptRichText(true);
}
