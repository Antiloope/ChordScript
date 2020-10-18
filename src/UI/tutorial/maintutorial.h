#ifndef MAINTUTORIAL_H
#define MAINTUTORIAL_H

#include <QWidget>
#include <QDialog>

namespace CS {
namespace UI {

class MainTutorial : public QDialog
{
    Q_OBJECT
public:
    explicit MainTutorial(QWidget *parent = nullptr);
};

}
}

#endif // MAINTUTORIAL_H
