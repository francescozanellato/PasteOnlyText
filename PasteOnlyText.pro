#-------------------------------------------------
#
# Project created by QtCreator 2013-07-26T15:06:09
#
#-------------------------------------------------
#*
#* Author: Francesco Paolo Luca Zanellato
#* Copyright (C) 2015-2025 Francesco Zanellato
#*
#* This program is free software: you can redistribute it and/or modify
#* it under the terms of the GNU General Public License as published by
#* the Free Software Foundation, either version 3 of the License, or
#* (at your option) any later version.
#*
#* This program is distributed in the hope that it will be useful,
#* but WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#* GNU General Public License for more details.
#*
#* You should have received a copy of the GNU General Public License
#* along with this program. If not, see <https://www.gnu.org/licenses/>.
#*/


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
