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

    if (m_timer->elapsed() > 500) {
        m_events.clear();
        m_timer->restart();
    }

    if (event->key() == Qt::Key_Control
            || event->key() == Qt::Key_Alt
            || event->key() == Qt::Key_Meta
            || event->key() == Qt::Key_AltGr ) {
        m_timer->restart();
        return;
    }

    m_events.append(KeyEvent{event->key(),
                             KeyEvent::Press,
                             event->modifiers()});

    QString result;
    result += "[";
    for (const auto& event : m_events) {
        result += "(";
        result += QString::number(event.key);
        result += ",";
        result += QString::number(event.type);
        result += ",";
        result += event.modifier.testFlag(Qt::ControlModifier) ? "CTRL," : "";
        result += event.modifier.testFlag(Qt::AltModifier) ? "ALT," : "";
        result += event.modifier.testFlag(Qt::MetaModifier) ? "Meta," : "";
        result += ")";
    }
    result += "]";

    if (m_events.size() >= 4
            && m_events[0].key == Qt::Key_Control
            && m_events[0].type == KeyEvent::Press
            && m_events[1].key == Qt::Key_X
            && m_events[1].type == KeyEvent::Press
            && m_events[2].key == Qt::Key_X
            && m_events[2].type == KeyEvent::Release
            && m_events[3].key == Qt::Key_C
            && m_events[3].type == KeyEvent::Press)
    {
        qDebug() << "Exiting";
    }

    qDebug() << result;
    event->accept();
    m_timer->restart();
}

void TextAreaKeyHandler::keyReleaseEvent(QKeyEvent *event)
{
    if (!m_document)
        return;
     event->accept();
}

