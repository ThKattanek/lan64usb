#-------------------------------------------------
#
# Project created by QtCreator 2015-01-09T22:23:51
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = lan64-send
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../lan64tool/lan64usb_class.cpp \
    ../../lan64tool/hiddata.c

linux-g++-32{
TARGET = lan64-send
LIBS += -lusb
}

linux-g++-64{
TARGET = lan64-send
LIBS += -lusb
}

win32-g++{
TARGET = lan64-send
LIBS += -lhid -lsetupapi
}

HEADERS += \
    ../../lan64tool/hiddata.h

# Installer

linux-g++-64 | linux-g++-32 | linux-g++{

target.path = /usr/bin/
INSTALLS += target

}
