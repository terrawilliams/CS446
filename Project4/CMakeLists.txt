cmake_minimum_required(VERSION 3.13)
project(Project4)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)
add_executable(Project4 main.cpp configuration.h configuration.cpp MetaData.h MetaData.cpp PCB.h PCB.cpp Timer.h Timer.cpp PCBStates.h)
target_link_libraries (Project4 ${CMAKE_THREAD_LIBS_INIT})