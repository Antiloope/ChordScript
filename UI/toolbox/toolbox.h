#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QToolBox>
#include <QWidget>

namespace CS {
namespace UI {

class SoundVisualizer;

class ToolBox : public QToolBox
{
    Q_OBJECT
public:
    explicit ToolBox(QWidget *parent = nullptr);
private slots:
private:
    SoundVisualizer* _soundVisualizer;

signals:

};

}
}

#endif // TOOLBOX_H
