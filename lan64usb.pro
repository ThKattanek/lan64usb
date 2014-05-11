#-------------------------------------------------
#
# Project created by QtCreator 2014-05-10T21:19:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lan64usb
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    lan64usb.cpp \
    hiddata.c

HEADERS  += mainwindow.h \
    lan64usb.h \
    hiddata.h

FORMS    += mainwindow.ui

linux-g++-32{
DESTDIR = "bin/linux_32bit"
TARGET = lan64tool
LIBS += -lusb
}

linux-g++-64{
TARGET = lan64tool
DESTDIR = "bin/linux_64bit"
LIBS += -lusb
}

win32-g++{
TARGET = lan64tool
DESTDIR = "bin/win_x32"
LIBS += -lhid -lsetupapi
}

OTHER_FILES += \
    firmware/Makefile \
    firmware/main.c \
    firmware/usbconfig.h \
    doc/schaltplan.pdf \
    c64/lan64.asm
