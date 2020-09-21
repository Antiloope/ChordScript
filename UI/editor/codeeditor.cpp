#include "codeeditor.h"
#include <QTextBlock>
#include <QPainter>
#include <QScrollBar>
#include "linenumberarea.h"
#include "syntaxhighlighter.h"
#include "UI/uidefinitions.h"

using namespace CS::UI;

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent) {
    lineNumberArea = new LineNumberArea(this);

    _highlighter = new SyntaxHighlighter(this->document());

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    setFocus();
    setLineWrapMode(LineWrapMode::NoWrap);

    setFont(UiDefinitions::getInstance()->getFont(FontId::Code));
    setStyleSheet(
        "QPlainTextEdit{"
            "selection-background-color:" + UiDefinitions::getInstance()->getColorRGB(ColorId::Primary) + ";"
            "border:none;"
        "}");
    setPlainText("# Type code here!");
    selectAll();

    zoomIn(5);

    setSizePolicy(QSizePolicy::Policy::Maximum,QSizePolicy::Policy::Maximum);

    setTabStopDistance( 4 * fontMetrics().horizontalAdvance(' ') );

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(selectionChanged()),this, SLOT(setSelectedText()));
}

void CodeEditor::wheelEvent(QWheelEvent *event) {
    if( event->modifiers() == Qt::ControlModifier )
    {
        if(event->delta() > 0)
            zoomIn(1);
        else
            zoomOut(1);
    }
    else if( event->modifiers() == Qt::AltModifier )
    {
        this->horizontalScrollBar()->event(event);
    }
    else
    {
        if( event->angleDelta().x() )
            this->horizontalScrollBar()->event(event);
        this->verticalScrollBar()->event(event);
    }
    event->accept();
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 6 + fontMetrics().horizontalAdvance('9') * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(UiDefinitions::getInstance()->getColor(ColorId::Light));

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(UiDefinitions::getInstance()->getColor(ColorId::Lightest)));
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    auto font = UiDefinitions::getInstance()->getFont(FontId::Global);
    font.setPixelSize(bottom-top-8);
    font.setBold(true);

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(UiDefinitions::getInstance()->getColor(ColorId::LightGray));
            painter.setFont(font);
            painter.drawText(0, top+2, lineNumberArea->width()-4, painter.fontMetrics().height()+10,
                             Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::setSelectedText() {
    _selectedText = "";
    for( auto i = this->textCursor().selectionStart(); i < this->textCursor().selectionEnd(); i++)
    {
        _selectedText += this->toPlainText()[i];
    }
}

QString CodeEditor::getText() const {
    if( _selectedText == "" )
        return this->toPlainText();
    return _selectedText;
}
