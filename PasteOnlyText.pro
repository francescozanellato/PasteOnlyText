#-------------------------------------------------
#
# Project created by QtCreator 2013-07-26T15:06:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PasteOnlyText
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogimagecroppreview.cpp

HEADERS  += mainwindow.h \
    dialogimagecroppreview.h

FORMS    += mainwindow.ui \
    dialogimagecroppreview.ui

# ICON in executable:
RC_ICONS = icon.ico

QMAKE_APPLE_DEVICE_ARCHS = x86_64 x86_64h arm64
