#ifndef TEXT_HIGHLIGHTER_H
#define TEXT_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class Text_Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Text_Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    QTextCharFormat preprocessingCommandFormat;
};

#endif // TEXT_HIGHLIGHTER_H
