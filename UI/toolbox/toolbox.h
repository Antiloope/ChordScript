#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QToolBox>
#include <QWidget>

namespace CS {
namespace UI {

class ToolBox : public QToolBox
{
    Q_OBJECT
public:
    explicit ToolBox(QWidget *parent = nullptr);
protected:
private slots:
private:

signals:

};

}
}

#endif // TOOLBOX_H
