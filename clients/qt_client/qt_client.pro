#-------------------------------------------------
#
# Project created by QtCreator 2016-02-18T13:36:13
#
#-------------------------------------------------

QT       += core network

TARGET = qt_client
TEMPLATE = app
CONFIG += c++11

macx {
    QMAKE_MAC_SDK = macosx10.11
    QMAKE_CXXFLAGS  += -Wno-inconsistent-missing-override
}

SOURCES += \
    src/main.cpp \
    src/network/clientsession.cpp \
    src/network/etat/abstractstate.cpp \
    src/network/etat/disconnectedstate.cpp \
    src/network/etat/readystate.cpp \
    src/network/etat/waitingstate.cpp \
    src/network/etat/workingstate.cpp \
    src/calculthread.cpp

HEADERS  += \
    src/network/clientsession.h \
    src/network/etat/abstractstate.h \
    src/network/etat/disconnectedstate.h \
    src/network/etat/readystate.h \
    src/network/etat/waitingstate.h \
    src/network/etat/workingstate.h \
    src/const.h \
    src/calculthread.h

FORMS    +=
