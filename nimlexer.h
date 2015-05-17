#pragma once

#include <QString>

class SourceCodeStream;

/// Nim lexer
class NimLexer final
{
public:
    enum State {
        Default = -1,
        MultiLineString = 0
    };

    enum class TokenType {
        Keyword,
        Identifier,
        Comment,
        Documentation,
        StringLiteral,
        MultiLineStringLiteral,
        EndOfText
    };

    struct Token {
        Token(int b, int l, TokenType t)
            : begin(b), length(l), type(t)
        {}

        int begin;
        int length;
        TokenType type;
    };

    /// Constructor
    NimLexer(const QChar* text,
             int length,
             State state = State::Default );

    ~NimLexer();

    /// Return the next token or EndOfText
    Token next();

    /// Return the lexer current state
    State state() const { return m_state; }

private:
    Token onDefaultState();
    Token onMultiLineStringState();

    bool isSkipChar(SourceCodeStream* stream);

    bool matchCommentStart(SourceCodeStream* stream);
    Token readComment(SourceCodeStream* stream);

    bool matchDocumentationStart(SourceCodeStream* stream);
    Token readDocumentation(SourceCodeStream* stream);

    bool matchIdentifierOrKeywordStart(SourceCodeStream* stream);
    Token readIdentifierOrKeyword(SourceCodeStream* stream);

    bool matchStringLiteralStart(SourceCodeStream* stream);
    Token readStringLiteral(SourceCodeStream* stream);

    bool matchMultiLineStringLiteralStart(SourceCodeStream* stream);
    Token readMultiLineStringLiteral(SourceCodeStream* stream, bool moveForward = true);

    const QChar* m_text;
    int m_length;
    State m_state;
    SourceCodeStream* m_stream;
};
