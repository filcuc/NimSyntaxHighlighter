#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T21:51:08
#
#-------------------------------------------------

QT       += core gui quick qml widgets
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NimSyntaxHighlighter
TEMPLATE = app


SOURCES += main.cpp\
    nimsyntaxhighlighter.cpp \
    applicationlogic.cpp \
    textareakeyhandler.cpp \
    textcursor.cpp \
    nimlexer.cpp

HEADERS  += \
    nimsyntaxhighlighter.h \
    applicationlogic.h \
    textareakeyhandler.h \
    textcursor.h \
    nimlexer.h

FORMS    +=

RESOURCES += \
    resources.qrc
