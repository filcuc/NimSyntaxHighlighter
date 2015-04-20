#include "textcursor.h"

TextCursor::TextCursor(QTextDocument *document, QObject *parent)
    : QObject(parent)
    , m_cursor(new QTextCursor(document))
{
    setPosition(0);
}

TextCursor::~TextCursor()
{

}

int TextCursor::position() const
{
    return m_cursor->position();
}

void TextCursor::moveLeft()
{
    if (!m_cursor->atStart())
        setPosition(position() - 1);
}

void TextCursor::moveRight()
{
    if (!m_cursor->atEnd())
        setPosition(position() + 1);
}

void TextCursor::setPosition(int arg)
{
    if (m_cursor->position() == arg)
        return;
    m_cursor->setPosition(arg);
    emit positionChanged(arg);
}

