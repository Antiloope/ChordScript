#ifndef FINDER_H
#define FINDER_H

#include <QWidget>
#include <QFrame>

class QCheckBox;
class QLineEdit;

namespace CS {
namespace UI {

class Finder : public QFrame {
    Q_OBJECT
public:
    explicit Finder(QWidget *parent = nullptr);
    void setFocusFind();
private:
    QCheckBox* _caseSensitiveCheck;
    QCheckBox* _wholeWordsCheck;
    QLineEdit* _textToFind;
private slots:
    void findNext();
    void findPrevious();
signals:
    void find(bool,bool,bool,QString);
};

}
}

#endif // FINDER_H
