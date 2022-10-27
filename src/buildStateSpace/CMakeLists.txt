cmake_minimum_required(VERSION 3.8)

project(buildStateSpace)

include(CheckIncludeFile)

CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)

if (HAVE_UNISTD_H)
  add_definitions(-DHAVE_UNISTD_H)
endif()

find_package(LIBSBML REQUIRED)

include_directories(${LIBSBML_INCLUDE_DIR})

add_executable(buildStateSpace_DFS 
    buildStateSpace_DFS.cpp 
    anyoption.cpp anyoption.h
)

target_link_libraries(buildStateSpace_DFS BioModel ${LIBSBML_LIBRARY})

set_target_properties(buildStateSpace_DFS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})

add_executable(buildStateSpace_BFS
    buildStateSpace_BFS.cpp
    anyoption.cpp anyoption.h
)

target_link_libraries(buildStateSpace_BFS BioModel ${LIBSBML_LIBRARY})

set_target_properties(buildStateSpace_BFS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
