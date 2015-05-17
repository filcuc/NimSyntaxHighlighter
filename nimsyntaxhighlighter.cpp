#include "nimsyntaxhighlighter.h"
#include "nimlexer.h"
#include <QDebug>

NimSyntaxHighlighter::NimSyntaxHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    {
        QTextCharFormat format;
        format.setForeground(QColor("black"));
        m_format[IdentifierToken] = format;
    }

    {
        QTextCharFormat format;
        format.setForeground(QColor("red"));
        format.setFontWeight(QFont::DemiBold);
        m_format[KeywordToken] = format;
    }

    {
        QTextCharFormat format;
        format.setForeground(QColor("gray"));
        m_format[CommentToken] = format;
    }

    {
        QTextCharFormat format;
        format.setForeground(QColor("green"));
        m_format[StringLiteralToken] = format;
    }

    {
        QTextCharFormat format;
        format.setForeground(QColor("blue"));
        m_format[NumberLiteralToken] = format;
    }
}

void NimSyntaxHighlighter::highlightBlock(const QString& ref)
{
    int temp = previousBlockState();
    NimLexer::State lexerState = static_cast<NimLexer::State>(temp);
    NimLexer lexer(ref.data(), ref.length(), lexerState);
    NimLexer::Token token = lexer.next();
    while (true)
    {
        switch (token.type) {
        case NimLexer::TokenType::Identifier:
            setFormat(token.begin, token.length, m_format[IdentifierToken]);
            break;
        case NimLexer::TokenType::Keyword:
            setFormat(token.begin, token.length, m_format[KeywordToken]);
            break;
        case NimLexer::TokenType::StringLiteral:
        case NimLexer::TokenType::MultiLineStringLiteral:
            setFormat(token.begin, token.length, m_format[StringLiteralToken]);
            break;
        case NimLexer::TokenType::Comment:
        case NimLexer::TokenType::Documentation:
            setFormat(token.begin, token.length,m_format[CommentToken]);
            break;
        case NimLexer::TokenType::EndOfText:
            setCurrentBlockState(lexer.state());
            return;
        }
        token = lexer.next();
    }
}
