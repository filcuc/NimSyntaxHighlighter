#pragma once
#include <QQuickItem>
#include <QQuickTextDocument>
#include <QTimer>
#include <QElapsedTimer>
#include <QQueue>
#include <QTextCursor>
#include "textcursor.h"

class TextAreaKeyHandler : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument* document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(TextCursor* cursor READ textCursor NOTIFY textCursorChanged)

public:
    TextAreaKeyHandler();
    ~TextAreaKeyHandler();

    QQuickTextDocument* document() const;

    TextCursor* textCursor() const;

public slots:
    void setDocument(QQuickTextDocument* arg);

signals:
    void documentChanged(QQuickTextDocument* arg);

    void textCursorChanged(TextCursor* arg);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QQuickTextDocument *m_document;
    QScopedPointer<TextCursor> m_cursor;
    QScopedPointer<QElapsedTimer> m_timer;
    QQueue<QKeyEvent> m_events;
};


