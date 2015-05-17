#include "nimlexer.h"
#include <QSet>
#include <QRegularExpression>

class SourceCodeStream
{
public:
    SourceCodeStream(const QChar *text, int length)
        :m_text(text)
        ,m_textLength(length)
        ,m_position(0)
        ,m_markedPosition(0)
    {}

    inline void setAnchor()
    {
        m_markedPosition = m_position;
    }

    inline void move()
    {
        ++m_position;
    }

    inline void moveToEnd()
    {
        m_position = m_textLength;
    }

    inline int pos()
    {
        return m_position;
    }

    inline int length() const
    {
        return m_position - m_markedPosition;
    }

    inline int anchor() const
    {
        return m_markedPosition;
    }

    inline bool isEnd() const
    {
        return m_position >= m_textLength;
    }

    inline QChar peek(int offset = 0) const
    {
        int pos = m_position + offset;
        if (pos >= m_textLength)
            return QLatin1Char('\0');
        return m_text[pos];
    }

    inline QString value() const
    {
        const QChar *start = m_text + m_markedPosition;
        return QString(start, length());
    }

    inline QString value(int begin, int length) const
    {
        return QString(m_text + begin, length);
    }

private:
    const QChar *m_text;
    const int m_textLength;
    int m_position;
    int m_markedPosition;
};

NimLexer::NimLexer(const QChar* text, int length, State state)
    : m_text(text)
    , m_length(length)
    , m_state(state)
    , m_stream(new SourceCodeStream(text, length))
{}

NimLexer::~NimLexer()
{
    delete m_stream;
}

NimLexer::Token NimLexer::next()
{
    switch (m_state)
    {
    case State::MultiLineString:
        return onMultiLineStringState();
    default:
        return onDefaultState();
    }
}

NimLexer::Token NimLexer::onDefaultState()
{
    while (!m_stream->isEnd())
    {
        if (isSkipChar(m_stream)) {
            m_stream->move();
            continue;
        }
        if (isOperator(m_stream))
            return readOperator(m_stream);
        if (matchDocumentationStart(m_stream))
            return readDocumentation(m_stream);
        if (matchCommentStart(m_stream))
            return readComment(m_stream);
        if (matchMultiLineStringLiteralStart(m_stream))
            return readMultiLineStringLiteral(m_stream, true);
        if (matchStringLiteralStart(m_stream))
            return readStringLiteral(m_stream);
        if (matchIdentifierOrKeywordStart(m_stream))
            return readIdentifierOrKeyword(m_stream);
        m_stream->move();
    }

    return Token {0,0, TokenType::EndOfText };
}

NimLexer::Token NimLexer::onMultiLineStringState()
{
    if (m_stream->isEnd())
        return Token {0,0, TokenType::EndOfText };
    return readMultiLineStringLiteral(m_stream, false);
}

bool NimLexer::isSkipChar(SourceCodeStream* stream)
{
    static QSet<QChar> skipChars {' ', '\t'};
    return skipChars.contains(stream->peek());
}

bool NimLexer::isOperator(SourceCodeStream* stream)
{
    static QSet<QChar> operators {'+', '-', '*', '/', '\\', '<', '>', '!', '?', '^', '.',
                                  '|', '=', '%', '&', '$', '@', '~', ':' };
    return operators.contains(stream->peek());
}

NimLexer::Token NimLexer::readOperator(SourceCodeStream* stream)
{
    stream->setAnchor();
    stream->move();
    return Token(stream->anchor(), stream->length(), TokenType::Operator);
}

bool NimLexer::matchCommentStart(SourceCodeStream* stream)
{
    return stream->peek() == '#' && stream->peek(1) != '#';
}

NimLexer::Token NimLexer::readComment(SourceCodeStream* stream)
{
    stream->setAnchor();
    stream->moveToEnd();
    return Token(stream->anchor(), stream->length(), TokenType::Comment);
}

bool NimLexer::matchDocumentationStart(SourceCodeStream* stream)
{
    return stream->peek() == '#' && stream->peek(1) == '#';
}

NimLexer::Token NimLexer::readDocumentation(SourceCodeStream* stream)
{
    stream->setAnchor();
    stream->moveToEnd();
    return Token(stream->anchor(), stream->length(), TokenType::Documentation);
}

bool NimLexer::matchIdentifierOrKeywordStart(SourceCodeStream* stream)
{
    static QRegularExpression isLetter {"[a-zA-Z\x80-\xFF]"};
    return isLetter.match(stream->peek()).hasMatch();
}

NimLexer::Token NimLexer::readIdentifierOrKeyword(SourceCodeStream* stream)
{
    static QRegularExpression isLetter {"[a-zA-Z\x80-\xFF]"};
    static QSet<QString> keywords {"template", "include", // 7
                                   "method", "string", "import" // 6
                                   "while", "cbool", "tuple", "defer", // 5
                                   "cint", "case", "bool", "proc", "type",
                                   "else", "from", "enum", "when", // 4
                                   "int", "var", "for", "ref", // 3
                                   "in", "of", "if" }; // 2
    stream->setAnchor();
    stream->move();

    while (!stream->isEnd()) {
        const QChar& c = stream->peek();
        if (!(c == '_' || c.isDigit() || isLetter.match(c).hasMatch()))
            break;
        stream->move();
    }

    QString value = stream->value();
    bool isKeyword = keywords.contains(value);

    return Token (stream->anchor(),
                  stream->length(),
                  isKeyword ? TokenType::Keyword : TokenType::Identifier );
}

bool NimLexer::matchStringLiteralStart(SourceCodeStream* stream)
{
    return stream->peek() == '"';
}

NimLexer::Token NimLexer::readStringLiteral(SourceCodeStream* stream)
{
    stream->setAnchor();
    stream->move();

    while (!stream->isEnd()) {
        if (stream->peek() != '\\' && stream->peek(1) == '"') {
            stream->move();
            stream->move();
            break;
        }
        stream->move();
    }

    return Token (stream->anchor(),
                  stream->length(),
                  TokenType::StringLiteral);
}

bool NimLexer::matchMultiLineStringLiteralStart(SourceCodeStream* stream)
{
    return stream->peek() == '"'
            && stream->peek(1) == '"'
            && stream->peek(2) == '"';
}

NimLexer::Token NimLexer::readMultiLineStringLiteral(SourceCodeStream* stream, bool moveForward)
{
    m_state = State::MultiLineString;
    stream->setAnchor();

    // Move ahead of 3 chars
    if (moveForward) {
        stream->move();
        stream->move();
        stream->move();
    }

    while (!stream->isEnd()) {
        if (stream->peek() == '"'
                && stream->peek(1) == '"'
                && stream->peek(2) == '"') {
            stream->move();
            stream->move();
            stream->move();
            m_state = State::Default;
            break;
        }
        stream->move();
    }

    return Token (stream->anchor(),
                  stream->length(),
                  TokenType::MultiLineStringLiteral);
}
