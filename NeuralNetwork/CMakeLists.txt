cmake_minimum_required(VERSION 3.19)
project(OCR)

set(CMAKE_C_STANDARD 11)

set(PROJECT_HEADERS
        Network.c
        NetworkTraining.c
        GatesTrainingData.c
        )

set(PROJECT_SOURCES
        Network.c
        NetworkTraining.c
        GatesTrainingData.c
        main.c
        )

add_executable(OCR ${PROJECT_SOURCES} ${PROJECT_HEADERS})
