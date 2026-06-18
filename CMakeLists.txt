cmake_minimum_required(VERSION 4.0)
project(Signal_Analyser)

set(CMAKE_CXX_STANDARD 20)

add_executable(Signal_Analyser main.cpp
        file/mp3Reader.cpp
        file/mp3Reader.h
        math/FourierTransform.cpp
        math/complex.h)

target_include_directories(Signal_Analyser PRIVATE include)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()
