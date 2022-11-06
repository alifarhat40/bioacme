# Copyright (C) 2019 - 2022 by Pedro Mendes, Rector and Visitors of the 
# University of Virginia, University of Heidelberg, and University 
# of Connecticut School of Medicine. 
# All rights reserved. 

# Copyright (C) 2017 - 2018 by Pedro Mendes, Virginia Tech Intellectual 
# Properties, Inc., University of Heidelberg, and University of 
# of Connecticut School of Medicine. 
# All rights reserved. 

# Copyright (C) 2012 - 2016 by Pedro Mendes, Virginia Tech Intellectual 
# Properties, Inc., University of Heidelberg, and The University 
# of Manchester. 
# All rights reserved. 

# Locate libsbml
# This module defines:
# LIBSBML_INCLUDE_DIR, where to find the headers
#
# LIBSBML_LIBRARY, LIBSBML_LIBRARY_DEBUG
# LIBSBML_FOUND, LIBSBML_LIBRARY_NAME

include(GNUInstallDirs)
include(LibFindMacros)

if(UNIX OR CYGWIN OR MINGW)
  set(LIBSBML_LIBRARY_NAME sbml)
else()
  set(LIBSBML_LIBRARY_NAME libsbml)
endif()

if (NOT LIBSBML_SHARED)
  set(LIBSBML_LIBRARY_NAME "${LIBSBML_LIBRARY_NAME}-static")
endif()

message (VERBOSE "Looking for ${LIBSBML_LIBRARY_NAME}")

string(TOUPPER ${CMAKE_PROJECT_NAME} _UPPER_PROJECT_NAME)
set(_PROJECT_DEPENDENCY_DIR ${_UPPER_PROJECT_NAME}_DEPENDENCY_DIR)

find_package(${LIBSBML_LIBRARY_NAME} CONFIG
PATHS ${CMAKE_INSTALL_FULL_LIBDIR}/cmake
        $ENV{LIBSBML_DIR}/lib/cmake
        $ENV{LIBSBML_DIR}/cmake
        $ENV{LIBSBML_DIR}
        ${${_PROJECT_DEPENDENCY_DIR}}/lib/cmake
        ${${_PROJECT_DEPENDENCY_DIR}}/lib64/cmake
        ${CONAN_LIB_DIRS_LIBSBML}/cmake
PATH_SUFFIXES sbml/lib/cmake libsbml/lib/cmake
)


if (${LIBSBML_LIBRARY_NAME}_FOUND)

  get_target_property(LIBSBML_LIBRARY ${LIBSBML_LIBRARY_NAME} LOCATION)
  get_target_property(LIBSBML_INCLUDE_DIR ${LIBSBML_LIBRARY_NAME} INTERFACE_INCLUDE_DIRECTORIES)
  if (NOT LIBSBML_INCLUDE_DIR)
    get_filename_component(LIB_PATH ${LIBSBML_LIBRARY} DIRECTORY)
    file(TO_CMAKE_PATH ${LIB_PATH}/../include LIBSBML_INCLUDE_DIR)  
    get_filename_component (LIBSBML_INCLUDE_DIR ${LIBSBML_INCLUDE_DIR} REALPATH)
  endif()
  get_target_property(LIBSBML_VERSION ${LIBSBML_LIBRARY_NAME} VERSION)


  get_target_property(LIBSBML_INTERFACE_LINK_LIBRARIES ${LIBSBML_LIBRARY_NAME} INTERFACE_LINK_LIBRARIES)

  if (NOT LIBSBML_INTERFACE_LINK_LIBRARIES)
  get_target_property(LIBSBML_INTERFACE_LINK_LIBRARIES ${LIBSBML_LIBRARY_NAME} IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE)
  endif()

  if (NOT LIBSBML_INTERFACE_LINK_LIBRARIES)
  get_target_property(LIBSBML_INTERFACE_LINK_LIBRARIES ${LIBSBML_LIBRARY_NAME} IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG)
  endif()

  if (NOT LIBSBML_INTERFACE_LINK_LIBRARIES)
  get_target_property(LIBSBML_INTERFACE_LINK_LIBRARIES ${LIBSBML_LIBRARY_NAME} IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG)
  endif()

  if (LIBSBML_INTERFACE_LINK_LIBRARIES)
    set(LIBSBML_LIBRARY ${LIBSBML_LIBRARY} ${LIBSBML_INTERFACE_LINK_LIBRARIES})
  endif (LIBSBML_INTERFACE_LINK_LIBRARIES)

  
  foreach (library ${LIBSBML_INTERFACE_LINK_LIBRARIES})
  
    string(FIND "${library}" "::" index)

    if (${index} GREATER 0)
      # found dependent library
      string(SUBSTRING "${library}" 0 ${index} DEPENDENT_NAME)
      message(VERBOSE "Looking for dependent library: ${DEPENDENT_NAME}")
      libfind_package(LIBSBML ${DEPENDENT_NAME})
    endif()
  
  endforeach()
else()

    find_path(LIBSBML_INCLUDE_DIR sbml/SBase.h
        PATHS ${CMAKE_INSTALL_FULL_INCLUDEDIR}
            $ENV{LIBSBML_DIR}/include
            $ENV{LIBSBML_DIR}
            ${${_PROJECT_DEPENDENCY_DIR}}
            ${${_PROJECT_DEPENDENCY_DIR}}/include
            ~/Library/Frameworks
            /Library/Frameworks
        PATH_SUFFIXES sbml/include libsbml/include
    )

    if (NOT LIBSBML_INCLUDE_DIR)
        message(FATAL_ERROR "libsbml include dir not found not found!")
    endif (NOT LIBSBML_INCLUDE_DIR)


    find_library(LIBSBML_LIBRARY 
        NAMES ${LIBSBML_LIBRARY_NAME}
        PATHS   ${CMAKE_INSTALL_FULL_LIBDIR}
                $ENV{LIBSBML_DIR}/lib
                $ENV{LIBSBML_DIR}
                ${${_PROJECT_DEPENDENCY_DIR}}
                ${${_PROJECT_DEPENDENCY_DIR}}/lib64
                ${${_PROJECT_DEPENDENCY_DIR}}/lib
                ${CONAN_LIB_DIRS_LIBSBML}
                $ENV{HOME}/Library/Frameworks
                /Library/Frameworks
        PATH_SUFFIXES sbml/lib libsbml/lib
    )

    if (NOT LIBSBML_LIBRARY)
        message(FATAL_ERROR "LIBSBML library not found!")
    endif (NOT LIBSBML_LIBRARY)

    foreach (library BZ2::BZ2;ZLIB::ZLIB;LIBXML::LIBXML)
  
      string(FIND "${library}" "::" index)

      if (${index} GREATER 0)
        string(SUBSTRING "${library}" 0 ${index} DEPENDENT_NAME)
        message(VERBOSE "Looking for dependent library: ${DEPENDENT_NAME}")
        libfind_package(LIBSBML ${DEPENDENT_NAME})
      endif()
  
  endforeach()

  add_library(${LIBSBML_LIBRARY_NAME} UNKNOWN IMPORTED)
  set_target_properties(${LIBSBML_LIBRARY_NAME} 
  PROPERTIES
      IMPORTED_LOCATION ${LIBSBML_LIBRARY}
      INTERFACE_INCLUDE_DIRECTORIES ${LIBSBML_INCLUDE_DIR}
  )

endif()

if (LIBSBML_INCLUDE_DIR)
  libfind_version_header(LIBSBML "sbml/common/libsbml-version.h" "LIBSBML_DOTTED_VERSION")
endif()

# set static on the library on windows
if ((WIN32 AND NOT CYGWIN) AND LIBSBML_INCLUDE_DIR AND LIBSBML_LIBRARY MATCHES "static")
  set_target_properties(${LIBSBML_LIBRARY_NAME} PROPERTIES 
  INTERFACE_COMPILE_DEFINITIONS "LIBSBML_STATIC=1;LIBLAX_STATIC=1;LIBSBML_EXPORTS;LIBLAX_EXPORTS"
)

endif()

# handle the QUIETLY and REQUIRED arguments and set LIBSBML_FOUND to TRUE if 
# all listed variables are TRUE

set(LIBSBML_PROCESS_INCLUDES LIBSBML_INCLUDE_DIR)
set(LIBSBML_PROCESS_LIBS LIBSBML_LIBRARY)
libfind_process(LIBSBML)
