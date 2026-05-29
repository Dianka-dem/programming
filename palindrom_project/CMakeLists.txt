cmake_minimum_required(VERSION 3.10)
project(PalindromFinder C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(palindrom 
    src/main.c 
    src/palindrom.c 
    src/logger.c
)
target_link_libraries(palindrom PRIVATE Threads::Threads)

enable_testing()
add_executable(unit_tests 
    tests/test_main.c 
    src/palindrom.c 
    src/logger.c
)
target_link_libraries(unit_tests PRIVATE Threads::Threads)
add_test(NAME RunTests COMMAND unit_tests)
