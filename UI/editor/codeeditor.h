#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>

namespace CS {
namespace UI {

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    QString getText() const;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *e) override;
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void setSelectedText();
private:
    QWidget *lineNumberArea;
    QString _selectedText;
signals:
};

}
}

#endif // CODEEDITOR_H
