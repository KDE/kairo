TEMPLATE = app
TARGET = kairo

include(../3rdparty/kirigami/kirigami.pri)

INCLUDEPATH += .
QT += quick multimedia
CONFIG += c++11

# Input
HEADERS += *.h
SOURCES += *.cpp
RESOURCES += *.qrc
