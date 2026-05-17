cmake_minimum_required(VERSION 3.10)

project(hello_cmake)

option(BUILD_SHARED_LIBS "Собирать статическую библиотеку" OFF)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(LIB_SOURCES 
    ${SRC_DIR}/smarthome.c
)

set(MAIN_SOURCES 
    ${SRC_DIR}/main.c
)

include_directories(include)

add_library(smarthome_lib ${LIB_SOURCES})

add_executable(main ${MAIN_SOURCES})

target_link_libraries(main smarthome_lib)