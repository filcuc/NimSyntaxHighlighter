#pragma once

#include "nimsyntaxhighlighter.h"
#include <QQuickTextDocument>

class ApplicationLogic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    explicit ApplicationLogic(QObject *parent = 0);

    ~ApplicationLogic();

    Q_INVOKABLE void openFile(const QString& path);
    Q_INVOKABLE void onOpenTriggered();
    Q_INVOKABLE void setDocument(QQuickTextDocument* document);

    QString text() const;

public slots:
    void setText(QString text);

signals:
    void textChanged(QString text);

private:
    QString m_text;
};
