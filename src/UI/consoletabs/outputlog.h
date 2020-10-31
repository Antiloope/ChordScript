#ifndef OUTPUTLOG_H
#define OUTPUTLOG_H

#include <QWidget>
#include <QTextBrowser>

namespace CS {
namespace UI {

class OutputLog : public QTextBrowser {
    Q_OBJECT
public:
    explicit OutputLog(QWidget *parent = nullptr);
private slots:
    void scrollToBottom();
};

}
}

#endif // OUTPUTLOG_H
