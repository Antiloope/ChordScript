#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QToolBox>
#include <QWidget>

namespace CS {
namespace UI {

class SoundAnalyzer;

class ToolBox : public QToolBox
{
    Q_OBJECT
public:
    explicit ToolBox(QWidget *parent = nullptr);
private slots:
private:
    SoundAnalyzer* _soundAnalyzer;

signals:

};

}
}

#endif // TOOLBOX_H
