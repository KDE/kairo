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
