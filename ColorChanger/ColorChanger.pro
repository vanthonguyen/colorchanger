#-------------------------------------------------
#
# Project created by QtCreator 2016-06-21T15:11:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorChanger
TEMPLATE = app

PKGCONFIG += openc
LIBS += $$system( pkg-config --libs opencv )
INCLUDEPATH += $$system( pkg-config --cflags opencv )

SOURCES += main.cpp\
        colorchanger.cpp \
    imagelabel.cpp \
    colorhsv.cpp \
    pointhsv.cpp

HEADERS  += colorchanger.h \
    imagelabel.h \
    imageutils.h \
    colorhsv.h \
    pointhsv.h

FORMS    += colorchanger.ui

