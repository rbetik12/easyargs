cmake_minimum_required(VERSION 3.17)
project(easyargs C)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE
        include/easyargs.h
        easyargs.cpp
        )

include_directories(include)

add_library(easyargs ${SOURCE})