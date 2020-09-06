#ifndef CONSOLETABS_H
#define CONSOLETABS_H

#include <QWidget>
#include <QTabWidget>

namespace CS {
namespace UI {


class ConsoleTabs : public QTabWidget {
    Q_OBJECT
public:
    explicit ConsoleTabs(QWidget *parent = nullptr);
private slots:
    void minimize();
    void maximize();
};

}
}

#endif // CONSOLETABS_H
