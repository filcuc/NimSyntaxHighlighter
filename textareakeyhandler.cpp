#include "textareakeyhandler.h"
#include <QDebug>

TextAreaKeyHandler::TextAreaKeyHandler()
    : m_document(nullptr)
    , m_cursor(nullptr)
    , m_timer(new QElapsedTimer())
{
    m_timer->start();
}

TextAreaKeyHandler::~TextAreaKeyHandler()
{
}

QQuickTextDocument* TextAreaKeyHandler::document() const
{
    return m_document;
}

TextCursor *TextAreaKeyHandler::textCursor() const
{
    return m_cursor.data();
}

void TextAreaKeyHandler::setDocument(QQuickTextDocument *arg)
{
    if (m_document == arg)
        return;

    m_document = arg;
    emit documentChanged(arg);

    m_cursor.reset(m_document ? new TextCursor(m_document->textDocument()) : nullptr);
    emit textCursorChanged(m_cursor.data());
}

void TextAreaKeyHandler::keyPressEvent(QKeyEvent *event)
{
    if (!m_document)
        return;

    if (m_timer->elapsed() > 250) {
        m_events.clear();
        m_timer->restart();
    }

    m_events.append(*event);


    for (const QKeyEvent& event : m_events) {
        qDebug() << event.key();
    }

    event->accept();
}

void TextAreaKeyHandler::keyReleaseEvent(QKeyEvent *event)
{
    if (!m_document)
        return;

    m_events.append(*event);
    event->accept();
}

