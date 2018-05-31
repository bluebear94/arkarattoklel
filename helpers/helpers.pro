TARGET = helper
TEMPLATE = lib
CONFIG += shared c++11 x86_64
QT += xml qml
QT -= gui
DEFINES += TF_DLL
DESTDIR = ../lib
INCLUDEPATH +=
DEPENDPATH  +=
LIBS +=
MOC_DIR = .obj/
OBJECTS_DIR = .obj/

include(../appbase.pri)

HEADERS += applicationhelper.h
HEADERS += markuphelper.h
SOURCES += applicationhelper.cpp
SOURCES += markuphelper.cpp
HEADERS += melpalthelper.h
SOURCES += melpalthelper.cpp
