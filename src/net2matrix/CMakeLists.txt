cmake_minimum_required(VERSION 3.8)

project(net2matrix)

find_package(LIBSBML REQUIRED)

include_directories("${LIBSBML_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} 
    net2matrix.cpp
    anyoption.cpp
    anyoption.h
)

target_link_libraries(${PROJECT_NAME} BioModel ${LIBSBML_LIBRARY})

# target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--unresolved-symbols=ignore-in-shared-libs")

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})