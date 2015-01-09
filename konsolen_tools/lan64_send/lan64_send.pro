#-------------------------------------------------
#
# Project created by QtCreator 2015-01-09T22:23:51
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = lan64_send
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

linux-g++-32{
DESTDIR = "../../bin/linux_32bit"
LIBS += -lusb
}

linux-g++-64{
DESTDIR = "../../bin/linux_64bit"
LIBS += -lusb
}

win32-g++{
DESTDIR = "../../bin/win_x32"
LIBS += -lhid -lsetupapi
}
