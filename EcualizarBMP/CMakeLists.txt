cmake_minimum_required(VERSION 3.6)
project(EcualizarBMP)

if(APPLE)
    include_directories(AFTER "/opt/X11/include")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I/opt/X11/include -L/opt/X11/lib -lX11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -L/opt/X11/lib -lX11")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(SOURCE_FILES
    CImg.h
    functions.cpp
    functions.h
    main.cpp
    test.out)

add_executable(EcualizarBMP ${SOURCE_FILES})