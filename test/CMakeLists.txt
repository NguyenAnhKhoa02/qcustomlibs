set(CMAKE_AUTOMOC ON)

enable_testing()

qt_add_executable(test
        test.cpp
)

target_link_libraries(test
    PRIVATE
        Qt6::Core
        Qt6::Test
#        qfuturewrapper
)

target_include_directories(test PRIVATE
    "${CMAKE_SOURCE_DIR}/cmake-install/include"
)

add_test(
        NAME test
        COMMAND test
)