TARGET = model
TEMPLATE = lib
CONFIG += shared c++11 x86_64
QT += sql qml
QT -= gui
DEFINES += TF_DLL
DESTDIR = ../lib
INCLUDEPATH += ../helpers sqlobjects mongoobjects
DEPENDPATH  += ../helpers sqlobjects mongoobjects
LIBS += -L../lib -lhelper
MOC_DIR = .obj/
OBJECTS_DIR = .obj/

include(../appbase.pri)
HEADERS += sqlobjects/entryobject.h
HEADERS += entry.h
SOURCES += entry.cpp
HEADERS += sqlobjects/userobject.h
HEADERS += user.h
SOURCES += user.cpp
LIBS += -L/usr/lib -L/usr/lib/x86_64-linux-gnu -largon2
HEADERS += sqlobjects/entryauditobject.h
HEADERS += entryaudit.h
SOURCES += entryaudit.cpp
HEADERS += sqlobjects/userauditobject.h
HEADERS += useraudit.h
SOURCES += useraudit.cpp
