#ifndef DOCUMENTATIONVIEWER_H
#define DOCUMENTATIONVIEWER_H

#include <QWidget>
#include <QFrame>

namespace CS {
namespace UI {

class DocumentationViewer : public QFrame
{
    Q_OBJECT
public:
    explicit DocumentationViewer(QWidget *parent = nullptr);
signals:
    void demo(QString code);
};

}
}

#endif // DOCUMENTATIONVIEWER_H
