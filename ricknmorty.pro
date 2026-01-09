QT+=widgets
QT+=network

TEMPLATE = app
TARGET = ricknmorty
INCLUDEPATH += .

CONFIG += debug

SOURCES += main.cpp \
	network.cpp \
	episodes.cpp \
	characters.cpp
	
HEADERS += network.h \
	episodes.h \
	characters.h

