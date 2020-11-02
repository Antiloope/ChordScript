#ifndef DOCUMENTATIONCONTENT_H
#define DOCUMENTATIONCONTENT_H

#include <QWidget>
#include <QFrame>
#include <QScrollArea>
#include <QBoxLayout>

namespace CS {
namespace UI {

const unsigned NUMBER_OF_SECTIONS = 6;

class DocumentationContent : public QFrame
{
    Q_OBJECT
public:
    explicit DocumentationContent(QWidget *parent = nullptr);
public slots:
    void display();
    void showDemo();
private:
    void initSection(unsigned section);
    unsigned _currentSection = 0;
    QFrame* _sections[NUMBER_OF_SECTIONS];
    QBoxLayout* _layout;
signals:
    void demo(QString code);
};

}
}

#endif // DOCUMENTATIONCONTENT_H
