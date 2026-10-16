cmake_minimum_required(VERSION 4.3)
project(qcustomlibs)

set(CMAKE_CXX_STANDARD 20)

find_package(
        Qt6 REQUIRED COMPONENTS

        Test
        Core
)

if(WIN32)
    set(CMAKE_INSTALL_PREFIX
            "${CMAKE_SOURCE_DIR}/cmake-install"
            CACHE PATH "Installation directory" FORCE
    )
endif()

add_subdirectory(src)
add_subdirectory(test)


