#pragma once

#include <QObject>
#include <QTextCursor>
#include <QTextDocument>

class TextCursor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)

public:
    explicit TextCursor(QTextDocument* document = nullptr, QObject* parent = nullptr);

    ~TextCursor();

    int position() const;

    void moveLeft();
    void moveRight();

public slots:
    void setPosition(int arg);

signals:
    void positionChanged(int arg);

private:
    QScopedPointer<QTextCursor> m_cursor;
};
