cmake_minimum_required(VERSION 3.8)

project(fbsdcme-r33)

#set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

set(CMAKE_INSTALL_PREFIX  ${CMAKE_BINARY_DIR}/bin)
# set(LIBSBML_ROOT "C:\\Program Files (x86)\\libsbml")

# set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
# set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

# Remove 'lib' prefix for shared libraries on Windows
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

add_subdirectory(src/mxexp)
add_subdirectory(src/SOR)
add_subdirectory(src/BioModel)
add_subdirectory(src/net2matrix)
add_subdirectory(src/buildStateSpace)
