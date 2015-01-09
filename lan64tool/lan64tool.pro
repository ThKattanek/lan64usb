#-------------------------------------------------
#
# Project created by QtCreator 2014-05-10T21:19:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lan64tool
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    lan64tool.cpp \
    hiddata.c

HEADERS  += mainwindow.h \
    lan64tool.h \
    hiddata.h

FORMS    += mainwindow.ui

linux-g++-32{
DESTDIR = "../../bin/linux_32bit"
TARGET = lan64tool
LIBS += -lusb
}

linux-g++-64{
TARGET = lan64tool
DESTDIR = "../../bin/linux_64bit"
LIBS += -lusb
}

win32-g++{
TARGET = lan64tool
DESTDIR = "../../bin/win_x32"
LIBS += -lhid -lsetupapi
}
