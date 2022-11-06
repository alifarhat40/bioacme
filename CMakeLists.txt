cmake_minimum_required(VERSION 3.8)

project(bioacme)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif()

if (MSVC OR MINGW)
  list(APPEND CMAKE_SYSTEM_PREFIX_PATH  "C:/Program Files/GnuWin/" "C:/Program Files (x86)/GnuWin32/")
endif()

# add missing prefix for unordinary distros
if(UNIX)
  list(APPEND CMAKE_SYSTEM_PREFIX_PATH  "$ENV{HOME}/usr" "$ENV{HOME}/usr/local" "/usr/freeware" "/sw" "/opt/csw" "/opt/local" "/usr/lib/x86_64-linux-gnu" "/usr/lib/i386-linux-gnu")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)

add_subdirectory(src/mxexp)
add_subdirectory(src/SOR)
add_subdirectory(src/BioModel)
add_subdirectory(src/net2matrix)
add_subdirectory(src/buildStateSpace)
