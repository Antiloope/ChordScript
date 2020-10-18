#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>
#include <QMenuBar>

namespace CS {
namespace UI {

class TopBar : public QMenuBar {
    Q_OBJECT
public:
    explicit TopBar(QWidget *parent = nullptr);
};

}
}

#endif // TOPBAR_H
