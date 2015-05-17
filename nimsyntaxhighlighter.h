#pragma once

#include <QSyntaxHighlighter>
#include <QRegularExpressionMatch>
#include <QTextCharFormat>

class NimLexer;

class NimSyntaxHighlighter : public QSyntaxHighlighter
{
    enum Token { IdentifierToken = 0,
                 KeywordToken,
                 CommentToken,
                 StringLiteralToken,
                 NumberLiteralToken};
public:
    NimSyntaxHighlighter(QTextDocument* parent = 0);

protected:
    virtual void highlightBlock(const QString& text);

private:
    QHash<Token, QTextCharFormat> m_format;
};
