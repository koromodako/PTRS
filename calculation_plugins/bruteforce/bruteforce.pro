QT += core
QT -= gui

CONFIG += c++11

macx {
    QMAKE_MAC_SDK = macosx10.11
    QMAKE_CXXFLAGS  += -Wno-inconsistent-missing-override
}

TARGET = bruteforce
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    src/computer.cpp \
    src/joiner.cpp \
    src/main.cpp \
    src/splitter.cpp

HEADERS += \
    src/bruteforce_specs.h \
    src/computer.h \
    src/joiner.h \
    src/splitter.h
