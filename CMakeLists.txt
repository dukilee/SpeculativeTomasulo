cmake_minimum_required(VERSION 2.8.11)

include_directories(.)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


find_package(Qt5Widgets)

add_executable(speculativeTomasulo WIN32 main.cpp MainWidget.cpp Window.cpp Interpreter.cpp)

target_link_libraries(speculativeTomasulo Qt5::Widgets)
