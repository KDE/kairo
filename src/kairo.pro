# Copyright 2016 Kevin Ottens <ervin@kde.org>

# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

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

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
