#include "applicationlogic.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QQmlEngine>

ApplicationLogic::ApplicationLogic(QObject *parent)
    : QObject(parent)
{
}

ApplicationLogic::~ApplicationLogic()
{
}

void ApplicationLogic::openFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);
    auto text = stream.readAll();
    setText(text);
    file.close();
}

void ApplicationLogic::onOpenTriggered()
{
    auto fileName = QFileDialog::getOpenFileName(0, QString("Open"), QString(), QString("Nim Files (*.nim)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);
    auto text = stream.readAll();
    setText(text);
    file.close();
}

void ApplicationLogic::setDocument(QQuickTextDocument* document)
{
    auto highlighter = new NimSyntaxHighlighter(document->textDocument());
    Q_UNUSED(highlighter)
}

QString ApplicationLogic::text() const
{
    return m_text;
}

void ApplicationLogic::setText(QString text)
{
    if (m_text == text)
        return;

    m_text = text;
    emit textChanged(text);
}
