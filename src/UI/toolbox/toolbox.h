#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QToolBox>
#include <QWidget>

namespace CS {
namespace UI {

class SoundVisualizer;
class DocumentationViewer;

class ToolBox : public QToolBox
{
    Q_OBJECT
public:
    explicit ToolBox(QWidget *parent = nullptr);
private slots:
    void showTutorial();
    void openCSPage();
private:
    DocumentationViewer* _documentation;
signals:
    void demo(QString code);
};

}
}

#endif // TOOLBOX_H
