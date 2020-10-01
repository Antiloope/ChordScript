#include "syntaxhighlighter.h"
#include "interpteter/languageConstants.h"
#include "UI/uidefinitions.h"

using namespace CS::UI;

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
    UiDefinitions* def = UiDefinitions::getInstance();

    HighlightingRule rule;
    dataTypeFormat.setForeground(QBrush(def->getColor(ColorId::H_DataType)));
    dataTypeFormat.setFontWeight(QFont::Bold);
    QStringList dataTypeKeywords;
    QString tmp;
    tmp = "\\b" + QString(CS::Constants::BOOLEAN.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::Constants::S_NULL.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::Constants::SAMPLE.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::Constants::SOUND.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::Constants::NUMBER.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::Constants::ARGUMENT.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::Constants::GROUP.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::Constants::STRING.c_str()) + "\\b";
    dataTypeKeywords << tmp;

    foreach (const QString &pattern, dataTypeKeywords) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = dataTypeFormat;
        highlightingRules.append(rule);
    }

    keywordFormat.setForeground(QBrush(def->getColor(ColorId::H_ReservedKeyword)));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    tmp = "\\b" + QString(CS::Constants::FOR.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::Constants::IF.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::Constants::BREAK.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::Constants::RETURN.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::Constants::ELSE.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::Constants::TRUE.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::Constants::FALSE.c_str()) + "\\b";
    keywordPatterns << tmp;

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QBrush(def->getColor(ColorId::H_Function)));
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;

    highlightingRules.append(rule);
    singleLineCommentFormat.setForeground(QBrush(def->getColor(ColorId::H_Comment)));
    rule.pattern = QRegularExpression("#[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::highlightBlock(const QString& text) {
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
//    int startIndex = 0;
//    if (previousBlockState() != 1)
//        startIndex = text.indexOf(commentStartExpression);

//    while (startIndex >= 0) {
//        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
//        int endIndex = match.capturedStart();
//        int commentLength = 0;
//        if (endIndex == -1) {
//            setCurrentBlockState(1);
//            commentLength = text.length() - startIndex;
//        } else {
//            commentLength = endIndex - startIndex
//                            + match.capturedLength();
//        }
//        setFormat(startIndex, commentLength, multiLineCommentFormat);
//        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
//    }
}
