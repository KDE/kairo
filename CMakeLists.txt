# SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

project(kairo)
cmake_minimum_required(VERSION 3.2)
if (POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()


find_package(ECM REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

find_package(Qt5 REQUIRED Core Quick Multimedia Test)

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(FeatureSummary)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(KF5Kirigami2 ${REQUIRED_KF5_VERSION} CONFIG QUIET)
set_package_properties(KF5Kirigami2 PROPERTIES
    DESCRIPTION "KF5 Kirigami 2 is needed to provide the mobile UI components."
    TYPE REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)

install(
    PROGRAMS org.kde.kairo.desktop
    DESTINATION ${XDG_APPS_INSTALL_DIR}
)

install(
    FILES org.kde.kairo.appdata.xml
    DESTINATION ${KDE_INSTALL_METAINFODIR}
)
