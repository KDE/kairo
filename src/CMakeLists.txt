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
add_executable(kairo ${exec_SRCS})
target_link_libraries(kairo kairolib)
