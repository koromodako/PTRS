QT += core
QT -= gui

CONFIG += c++11

TARGET = bruteforce
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    splitter.cpp \
    joiner.cpp \
    computer.cpp

HEADERS += \
    splitter.h \
    joiner.h \
    bruteforce_specs.h \
    computer.h
