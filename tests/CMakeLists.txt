include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

macro(kairo_auto_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        add_test(${_testname} ${_testname})
        target_link_libraries(${_testname}
            Qt5::Test
            kairolib
        )
    endforeach(_testname)
endmacro(kairo_auto_tests)

kairo_auto_tests(
    circuitcontroltest
    circuitlistmodeltest
    circuitmodeltest
    circuitreadertest
    timercontroltest
    timermodeltest
    timernotificationcontroltest
)
