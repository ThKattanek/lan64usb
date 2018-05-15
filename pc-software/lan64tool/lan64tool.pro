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
    lan64usb_class.cpp \
    hiddata.c

HEADERS  += mainwindow.h \
    lan64usb_class.h \
    hiddata.h

FORMS    += mainwindow.ui

linux-g++-32{
TARGET = lan64tool
LIBS += -lusb
}

linux-g++-64{
TARGET = lan64tool
LIBS += -lusb
}

win32-g++{
# TARGET = lan64tool
LIBS += -lhid -lsetupapi
}

# Installer

linux-g++-64 | linux-g++-32 | linux-g++{

target.path = /usr/bin/
INSTALLS += target

}
