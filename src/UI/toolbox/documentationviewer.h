#ifndef DOCUMENTATIONVIEWER_H
#define DOCUMENTATIONVIEWER_H

#include <QWidget>
#include <QTextBrowser>

namespace CS {
namespace UI {

class DocumentationViewer : public QTextBrowser
{
    Q_OBJECT
public:
    explicit DocumentationViewer(QWidget *parent = nullptr);
};

}
}

#endif // DOCUMENTATIONVIEWER_H
