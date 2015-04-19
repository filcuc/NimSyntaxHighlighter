#include "nimsyntaxhighlighter.h"

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
    int i = 0;
    bool multiLineMatched = false;

    if (currentBlockState() != previousBlockState())
        setCurrentBlockState(previousBlockState());

    while (i < ref.count()) {
        if (previousBlockState() == MultiLineStringBlockState && !multiLineMatched) {
            if (matchMultiLineStringEnd(ref, i)) {
                multiLineMatched = true;
                setCurrentBlockState(DefaultBlockState);
            }
            continue;
        }
        if (matchSkipChar(ref, i))
            continue;
        if (matchComment(ref,i))
            continue;
        if (matchNumberLiteral(ref, i))
            continue;
        if (matchMultiLineStringLiteral(ref,i))
            continue;
        if (matchStringLiteral(ref, i))
            continue;
        if (matchIdentifier(ref, i))
            continue;
        ++i; // Nothing matched
    }
}

bool NimSyntaxHighlighter::matchMultiLineStringEnd(const QString& ref, int& i)
{
    static QTextCharFormat format = m_format[StringLiteralToken];

    int j = i;
    const int start = i;
    QChar previous;

    while (j < ref.length()) {
        if (ncharFollows('"', ref, j, 3) && previous != '\\') {
            i = j+3;
            setFormat(start, i-start, format);
            return true;
        }
        previous = ref[j];
        ++j;
    }

    setFormat(i, ref.count() - i, format);
    i = ref.count();
    return false;
}

bool NimSyntaxHighlighter::ncharFollows(const QChar& c, const QString& str, int pos, int n)
{
    int i = 0;
    while (i < n && (pos + i) < str.length()) {
        if (str[pos+i] != c)
            return false;
        ++i;
    }
    return i == n;
}

bool NimSyntaxHighlighter::matchComment(const QString& ref, int& i)
{
    static const QTextCharFormat format = m_format[CommentToken];
    // Test comment
    if (ref[i] == '#') {
        const int& length = ref.count() - i;
        setFormat(i, length, format);
        i += length;
        return true;
    }
    return false;
}

bool NimSyntaxHighlighter::matchSkipChar(const QString& ref, int& i)
{
    static bool firstTime = true;
    static QSet<QChar> skipChars {{' ', '+', '-',
                                   '*', '[', ']',
                                   '{', '}', '%',
                                   '\\', '\n', '\r',
                                   '\t'}};
    if (firstTime) {
        skipChars.squeeze();
        firstTime = false;
    }

    if (skipChars.contains(ref.at(i))) {
        ++i;
        return true;
    }
    return false;
}

bool NimSyntaxHighlighter::matchIdentifier(const QString& ref, int& i)
{
    static QRegularExpression isLetter {"[a-zA-Z\x80-\xFF]"};
    static bool firstTime = true;
    static  QSet<QString> keywords {"template", "include", // 7
                                    "method", "string", "import" // 6
                                    "while", "cbool", "tuple", "defer", // 5
                                    "cint", "case", "bool", "proc", "type",
                                    "else", "from", "enum", "when", // 4
                                    "int", "var", "for", // 3
                                    "in", "of", "if" }; // 2
    static const QTextCharFormat keywordFormat = m_format[KeywordToken];

    if (firstTime) {
        isLetter.optimize();
        keywords.squeeze();
        firstTime = false;
    }

    const QChar& c = ref[i];
    if (!isLetter.match(c).hasMatch())
        return false;

    const int start = i;
    ++i;

    while (i < ref.length()) {
        const QChar& c = ref[i];
        if (!(c == '_' || c.isDigit() || isLetter.match(c).hasMatch()))
            break;
        ++i;
    }

    bool isKeyword = keywords.contains(ref.mid(start, i - start));

    if (isKeyword)
        setFormat(start, i - start, keywordFormat);

    return true;
}

bool NimSyntaxHighlighter::matchNumberLiteral(const QString& ref, int& i)
{
    static const QTextCharFormat format = m_format[NumberLiteralToken];

    const QChar& c = ref[i];

    if (!c.isDigit())
        return false;

    const int start = i;
    ++i;

    while (i < ref.length()) {
        const QChar& c = ref[i];
        if (!c.isDigit())
            break;
        ++i;
    }

    setFormat(start, i - start, format);

    return true;
}

bool NimSyntaxHighlighter::matchStringLiteral(const QString& ref, int& i)
{
    static const QTextCharFormat format = m_format[StringLiteralToken];

    const QChar& c = ref[i];

    if (c != '"')
        return false;

    const int start = i;
    QChar previous = c;
    i++;

    while (i < ref.length()) {
        const QChar& c = ref[i];
        if (c == '"' && previous != '\\') {
            ++i;
            break;
        }
        previous = c;
        ++i;
    }

    setFormat(start, i - start, format);

    return true;
}

bool NimSyntaxHighlighter::matchMultiLineStringLiteral(const QString& ref, int& i)
{
    static const QTextCharFormat format = m_format[StringLiteralToken];
    const QChar& c = ref[i];

    // Lookahead of 2 chars
    if (!ncharFollows('"', ref, i, 3))
        return false;

    const int start = i;
    QChar previous = c;
    i++;

    bool foundEnd = false;

    while (i < ref.length()) {
        const QChar& c = ref[i];
        if (ncharFollows('"', ref, i, 3) && previous != '\\') {
            i += 3;
            foundEnd = true;
            break;
        }
        previous = c;
        ++i;
    }

    setFormat(start, i - start, format);

    if (!foundEnd)
        setCurrentBlockState(MultiLineStringBlockState);

    return true;
}

