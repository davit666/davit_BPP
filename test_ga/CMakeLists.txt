cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
project(test_ga)

set(INC_DIR /home/davit/Documents/files/GAlib_files/galib_2.4.7.orig/galib247)
set(LINK_DIR /home/davit/Documents/files/GAlib_files/galib_2.4.7.orig/galib247)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(ga)

add_executable(main ./src/ga_binpack_davit.cpp)
target_link_libraries(main ga)



