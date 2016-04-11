#-------------------------------------------------
#
# Project created by QtCreator 2016-02-18T13:23:13
#
#-------------------------------------------------

QT       += core network gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app
CONFIG += c++11

macx {
    QMAKE_MAC_SDK = macosx10.11
    QMAKE_CXXFLAGS  += -Wno-inconsistent-missing-override
}

SOURCES += \
    src/main.cpp \
    src/console/consolehandler.cpp \
    src/calculation/calculationmanager.cpp \
    src/network/networkmanager.cpp \
    src/calculation/calculation.cpp \
    src/network/clientsession.cpp \
    src/utils/abstractidentifiable.cpp \
    src/utils/logger.cpp \
    src/applicationmanager.cpp \
    src/plugins/pluginmanager.cpp \
    src/network/udpserver.cpp \
    src/network/tcpserver.cpp \
    src/network/etat/abstractstate.cpp \
    src/network/etat/disconnectedstate.cpp \
    src/network/etat/readystate.cpp \
    src/network/etat/waitingstate.cpp \
    src/network/etat/workingabouttostartstate.cpp \
    src/network/etat/workingstate.cpp \
    src/plugins/pluginprocess.cpp \
    src/calculation/fragment.cpp \
    src/ui/addcalculationwindow.cpp \
    src/ui/mainwindow.cpp \
    src/ui/widgetcalculs.cpp \
    src/ui/widgetclients.cpp \
    src/userinterface.cpp \
    src/ui/mainwindowcontroller.cpp

HEADERS  += \
    src/console/consolehandler.h \
    src/calculation/calculationmanager.h \
    src/network/networkmanager.h \
    src/calculation/calculation.h \
    src/network/clientsession.h \
    src/const.h \
    src/utils/abstractidentifiable.h \
    src/utils/logger.h \
    src/applicationmanager.h \
    src/plugins/pluginmanager.h \
    src/network/udpserver.h \
    src/network/tcpserver.h \
    src/network/etat/abstractstate.h \
    src/network/etat/disconnectedstate.h \
    src/network/etat/readystate.h \
    src/network/etat/waitingstate.h \
    src/network/etat/workingabouttostartstate.h \
    src/network/etat/workingstate.h \
    src/calculation/specs.h \
    src/plugins/pluginprocess.h \
    src/calculation/fragment.h \
    src/ui/addcalculationwindow.h \
    src/ui/mainwindow.h \
    src/ui/widgetcalculs.h \
    src/ui/widgetclients.h \
    src/userinterface.h \
    src/ui/mainwindowcontroller.h

FORMS    += src/ui/mainwindow.ui

# retrieve host & build information
DEFINES += QHOST_ARCH=\\\"$$QMAKE_HOST.arch\\\"
DEFINES += QHOST_OS=\\\"$$QMAKE_HOST.os\\\"
DEFINES += QHOST_CPU_COUNT=\\\"$$QMAKE_HOST.cpu_count\\\"
DEFINES += QHOST_NAME=\\\"$$QMAKE_HOST.name\\\"
DEFINES += QHOST_VERSION=\\\"$$QMAKE_HOST.version\\\"
DEFINES += QHOST_VERSION_STRING=\\\"$$QMAKE_HOST.version_string\\\"
DEFINES += QGIT_LAST_COMMIT=\\\"$$system(git rev-list HEAD | head -n 1)\\\"
DEFINES += QGIT_DIRTY=$$system(git status --porcelain | wc -l)
DEFINES += QGIT_BRANCH=\\\"$$system(git rev-parse --abbrev-ref HEAD)\\\"
