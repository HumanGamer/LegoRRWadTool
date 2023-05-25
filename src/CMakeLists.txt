cmake_minimum_required(VERSION 3.25)
project(LegoRRWadTool C)

set(CMAKE_C_STANDARD 11)

set(MAIN_SRC
    # Sources
    main.c
    wad.c

    # Headers
    wad.h
    types.h
)

add_executable(LegoRRWadTool ${MAIN_SRC})
