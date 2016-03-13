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
    src/plugins/calculationprocess.cpp \
    src/plugins/pluginmanager.cpp \
    src/calculation/calculation.cpp \
    src/utils/logger.cpp \
    src/utils/abstractidentifiable.cpp

HEADERS  += \
    src/network/clientsession.h \
    src/network/etat/abstractstate.h \
    src/network/etat/disconnectedstate.h \
    src/network/etat/readystate.h \
    src/network/etat/waitingstate.h \
    src/network/etat/workingstate.h \
    src/const.h \
    src/plugins/calculationprocess.h \
    src/plugins/pluginmanager.h \
    src/calculation/calculation.h \
    src/calculation/specs.h \
    src/utils/logger.h \
    src/utils/abstractidentifiable.h

FORMS    +=
