cmake_minimum_required(VERSION 3.10)
project(SystemMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include header files
include_directories(include)

# Gather all source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(SystemMonitor ${SOURCES})

# Link ncurses library
target_link_libraries(SystemMonitor ncurses)
