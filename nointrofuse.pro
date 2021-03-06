#-------------------------------------------------
#
# Project created by QtCreator 2015-03-02T12:05:39
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = nointrofuse
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    datparser.cpp \
    artworkscraper.cpp \
    parser.cpp \
    csvparser.cpp

HEADERS += \
    datparser.h \
    artworkscraper.h \
    utils.h \
    parser.h \
    csvparser.h
