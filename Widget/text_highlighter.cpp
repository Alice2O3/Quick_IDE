#include "text_highlighter.h"

Text_Highlighter::Text_Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    preprocessingCommandFormat.setForeground(QBrush(QColor(0, 0, 151)));
    QStringList preprocessingPatterns;
    preprocessingPatterns << "\\B\\#define\\b" << "\\B\\#include\\b" << "\\B\\#if\\b" <<
                             "\\B\\#elif\\b" << "\\B\\#ifndef\\b" << "\\B\\#ifdef\\b" <<
                             "\\B\\#endif\\b" << "\\B\\#undef\\b" << "\\B\\#error\\b" <<
                             "\\B\\#line\\b" << "\\B\\#pragma\\b";

    foreach (const QString &pattern, preprocessingPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = preprocessingCommandFormat;
        highlightingRules.append(rule);
    }

    keywordFormat.setForeground(QBrush(QColor(128, 128, 0)));

    QStringList keywordPatterns;
    keywordPatterns << "\\bif\\b" << "\\bint\\b" << "\\bfor\\b"  << "\\bdo\\b"
                    << "\\bnew\\b" << "\\btry\\b" << "\\basm\\b" << "\\belse\\b"
                    << "\\bchar\\b" << "\\bfloat\\b" << "\\blong\\b" << "\\bvoid\\b"
                    << "\\bshort\\b" << "\\bwhile\\b" << "\\bdouble\\b" << "\\bbreak\\b"
                    << "\\btypedef\\b" << "\\bregister\\b" << "\\bcontinue\\b" << "\\bcatch\\b"
                    << "\\bsigned\\b" << "\\bunsigned\\b" << "\\bauto\\b" << "\\bstatic\\b"
                    << "\\bextern\\b" << "\\bsizeof\\b" << "\\bdelete\\b" << "\\bthrow\\b"
                    << "\\bconst\\b" << "\\bclass\\b" << "\\bfriend\\b" << "\\breturn\\b"
                    << "\\bswitch\\b" << "\\bpublic\\b" << "\\bunion\\b" << "\\bgoto\\b"
                    << "\\boperator\\b" << "\\btemplate\\b" << "\\benum\\b" << "\\bprivate\\b"
                    << "\\bvolatile\\b" << "\\bthis\\b" << "\\bvirtual\\b" << "\\bcase\\b"
                    << "\\bdefault\\b" << "\\binline\\b" << "\\bprotected\\b" << "\\bstruct\\b"
                    << "\\bexplicit\\b" << "\\bnamespace\\b" << "\\busing\\b" << "\\btypename\\b"
                    << "\\bbool\\b";

    //Numbers
    numberFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b((0x[A-Fa-f0-9]*)|(\\d+(\\.\\d+)?)|(\\d+))\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    //Functions
    functionFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\b(?![0-9])[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    //Key Words
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    //Quotes
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("(\"[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*\")");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //Small Quotes
    rule.pattern = QRegularExpression("('[^'\\\\]*(?:\\\\.[^'\\\\]*)*')");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //Single Line Comment
    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    //Multi Line Comment
    multiLineCommentFormat.setForeground(Qt::red);
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");

}

void Text_Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1){
        startIndex = text.indexOf(commentStartExpression);
    }

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
