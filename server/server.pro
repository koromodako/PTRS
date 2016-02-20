#-------------------------------------------------
#
# Project created by QtCreator 2016-02-18T13:23:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/console/consolehandler.cpp \
    src/calculation/calculationmanager.cpp \
    src/network/networkmanager.cpp \
    src/utils/calculationfactory.cpp \
    src/calculation/calculationorder.cpp \
    src/calculation/calculation.cpp \
    src/calculation/calculationfragment.cpp \
    src/network/clientsession.cpp \
    src/utils/abstractidentifiable.cpp \
    src/utils/logger.cpp \
    src/applicationmanager.cpp

HEADERS  += \
    src/console/consolehandler.h \
    src/calculation/calculationmanager.h \
    src/network/networkmanager.h \
    src/utils/calculationfactory.h \
    src/calculation/calculationorder.h \
    src/calculation/calculation.h \
    src/calculation/calculationfragment.h \
    src/network/clientsession.h \
    src/const.h \
    src/utils/abstractidentifiable.h \
    src/utils/logger.h \
    src/applicationmanager.h

FORMS    +=
