#pragma once

#include <QSyntaxHighlighter>
#include <QRegularExpressionMatch>
#include <QTextCharFormat>

class NimSyntaxHighlighter : public QSyntaxHighlighter
{
    enum Token { IdentifierToken = 0,
                 KeywordToken,
                 CommentToken,
                 StringLiteralToken,
                 NumberLiteralToken};

    enum BlockState { DefaultBlockState = -1,
                      MultiLineStringBlockState = 0};
public:
    NimSyntaxHighlighter(QTextDocument* parent = 0);

protected:
    virtual void highlightBlock(const QString& text);

private:
    bool matchComment(const QString& str, int& i);
    bool matchSkipChar(const QString& str, int& i);
    bool matchIdentifier(const QString& str, int& i);
    bool matchNumberLiteral(const QString& str, int& i);
    bool matchStringLiteral(const QString& str, int& i);
    bool matchMultiLineStringLiteral(const QString& str, int& i);
    bool matchMultiLineStringEnd(const QString& ref, int& i);
    static bool ncharFollows(const QChar& c, const QString& str, int pos, int n = 1);

    QHash<Token, QTextCharFormat> m_format;
};
