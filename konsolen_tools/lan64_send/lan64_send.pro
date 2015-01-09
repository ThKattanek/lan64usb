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

SOURCES += main.cpp \
    ../../lan64tool/lan64usb_class.cpp \
    ../../lan64tool/hiddata.c

linux-g++-32{
TARGET = lan64_send
DESTDIR = "../../bin/linux_32bit"
LIBS += -lusb
}

linux-g++-64{
TARGET = lan64_send
DESTDIR = "../../bin/linux_64bit"
LIBS += -lusb
}

win32-g++{
TARGET = lan64_send
DESTDIR = "../../bin/win_x32"
LIBS += -lhid -lsetupapi
}

HEADERS += \
    ../../lan64tool/hiddata.h
