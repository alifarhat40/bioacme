cmake_minimum_required(VERSION 3.8)

project(ssor)

add_executable(${PROJECT_NAME} 
    ssor.cpp
    anyoption.cpp 
    anyoption.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})