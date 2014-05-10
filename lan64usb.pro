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

HEADERS  += mainwindow.h \

FORMS    += mainwindow.ui

linux-g++-32{
DESTDIR = "bin/linux_32bit"
TARGET = lan64tool
}

linux-g++-64{
TARGET = lan64tool
DESTDIR = "bin/linux_64bit"
}

win32-g++{
TARGET = lan64tool
DESTDIR = "bin/win_x32"
}

OTHER_FILES += \
    firmware/Makefile \
    firmware/main.c \
    firmware/usbconfig.h \
    doc/schaltplan.pdf \
    c64/lan64.asm
