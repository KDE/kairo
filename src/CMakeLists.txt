# SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

set(lib_SRCS
    circuitcontrol.cpp
    circuitlistmodel.cpp
    circuitmodel.cpp
    circuitreader.cpp
    timercontrol.cpp
    timermodel.cpp
    timernotificationcontrol.cpp
    soundcontrolinterface.cpp
    soundcontrol.cpp
    soundcontrolstub.cpp
)
add_library(kairolib STATIC ${lib_SRCS})
target_link_libraries(kairolib
    Qt5::Quick
    Qt5::Multimedia
)

set(exec_SRCS
    main.cpp
)
qt5_add_resources(exec_SRCS
    main.qrc
)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/kirigami/src)

add_executable(kairo ${exec_SRCS})
target_link_libraries(kairo kairolib)

install(TARGETS kairo DESTINATION ${KDE_INSTALL_BINDIR})
