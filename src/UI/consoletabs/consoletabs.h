#ifndef CONSOLETABS_H
#define CONSOLETABS_H

#include <QWidget>
#include <QTabWidget>

class QTextBrowser;

namespace CS {
namespace UI {

class Finder;

class ConsoleTabs : public QTabWidget {
    Q_OBJECT
public:
    explicit ConsoleTabs(QWidget *parent = nullptr);
    void setFocusFind();
private:
    QTextBrowser* _outputLog;
    Finder* _finder;
private slots:
    void findRequested(bool,bool,bool,QString);
signals:
    void find(bool,bool,bool,QString);
};

}
}

#endif // CONSOLETABS_H
