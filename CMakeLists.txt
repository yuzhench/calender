cmake_minimum_required(VERSION 3.10)
project(YuzhenchTimeSchedule)

set(CMAKE_CXX_STANDARD 11)

#define the data direction
set(DATA_DIR "${CMAKE_SOURCE_DIR}/data")
message("DATA_DIR is set to: ${DATA_DIR}")
add_definitions(-DDATA_DIR=\"${DATA_DIR}\")


# Add the executable
add_executable(yuzhench_time_schedule src/functionality.cpp)

# Find and link ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(yuzhench_time_schedule ${CURSES_LIBRARIES})

# Install the executable in /usr/local/bin
install(TARGETS yuzhench_time_schedule DESTINATION /usr/local/bin)

 