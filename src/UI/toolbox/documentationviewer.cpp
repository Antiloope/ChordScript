#include "documentationviewer.h"

using namespace CS::UI;

DocumentationViewer::DocumentationViewer(QWidget *parent) :
    QTextBrowser(parent)
{
    setSource(QUrl("./etc/Language-Documentation.html"));
    setOpenExternalLinks(true);
    setStyleSheet(
        "QTextBrowser {"
            "border:none;"
        "}");
}
