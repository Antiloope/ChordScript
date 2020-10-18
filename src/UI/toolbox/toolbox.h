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
    SoundVisualizer* _soundVisualizer;
    DocumentationViewer* _documentation;

signals:

};

}
}

#endif // TOOLBOX_H
