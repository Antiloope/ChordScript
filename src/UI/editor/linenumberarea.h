#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "UI/editor/codeeditor.h"

namespace CS {
namespace UI {

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor*);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent*) override;

private:
    CodeEditor *codeEditor;
};

}
}

#endif // LINENUMBERAREA_H
