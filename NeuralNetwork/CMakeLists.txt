cmake_minimum_required(VERSION 3.19)
project(OCR)

set(CMAKE_C_STANDARD 11)

set(PROJECT_HEADERS
        Network.h
        )

set(PROJECT_SOURCES
        Network.c
        )

add_executable(OCR ${PROJECT_SOURCES} ${PROJECT_HEADERS})
