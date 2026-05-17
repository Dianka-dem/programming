cmake_minimum_required(VERSION 3.10)
project(PaintProject)

# Установите стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найдите библиотеку SFML
find_package(sfml-graphics REQUIRED)
find_package(sfml-window REQUIRED)
find_package(sfml-system REQUIRED)

# Добавьте исполняемый файл
add_executable(paint
    main.cpp
    paint_core.cpp
)

# Подключите библиотеки SFML
target_link_libraries(paint
    sfml-graphics
    sfml-window
    sfml-system
)

# Если SFML не находится автоматически, раскомментируйте строку ниже
# и укажите путь к SFML вручную
# set(SFML_DIR "/usr/lib/cmake/SFML")
