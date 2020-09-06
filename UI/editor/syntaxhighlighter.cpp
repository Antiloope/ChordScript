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
    tmp = "\\b" + QString(CS::constants::sBoolean.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::constants::sNull.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::constants::sSample.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::constants::sSound.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::constants::sNumber.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::constants::sArgument.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::constants::sGroup.c_str()) + "\\b";
    dataTypeKeywords << tmp;
    tmp = "\\b" + QString(CS::constants::sString.c_str()) + "\\b";
    dataTypeKeywords << tmp;

    foreach (const QString &pattern, dataTypeKeywords) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = dataTypeFormat;
        highlightingRules.append(rule);
    }

    keywordFormat.setForeground(QBrush(def->getColor(ColorId::H_ReservedKeyword)));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    tmp = "\\b" + QString(CS::constants::sFor.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::constants::sIf.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::constants::sBreak.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::constants::sReturn.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::constants::sElse.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::constants::sTrue.c_str()) + "\\b";
    keywordPatterns << tmp;
    tmp = "\\b" + QString(CS::constants::sFalse.c_str()) + "\\b";
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
