#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T14:38:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server_ihm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    widgetclients.cpp \
    widgetcalculs.cpp

HEADERS  += mainwindow.h \
    widgetclients.h \
    widgetcalculs.h

FORMS    += mainwindow.ui
