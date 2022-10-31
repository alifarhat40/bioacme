cmake_minimum_required(VERSION 3.8)

project(acme)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)

# Remove 'lib' prefix for shared libraries on Windows
if(WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

add_subdirectory(src/mxexp)
add_subdirectory(src/SOR)
add_subdirectory(src/BioModel)
add_subdirectory(src/net2matrix)
add_subdirectory(src/buildStateSpace)
