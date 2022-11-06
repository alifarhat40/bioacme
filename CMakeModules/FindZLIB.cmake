# Copyright (C) 2022 by Pedro Mendes, Rector and Visitors of the 
# University of Virginia, University of Heidelberg, and University 
# of Connecticut School of Medicine. 
# All rights reserved. 

# - Try to find the Zlib XML parsing library 

# Once done this will define
#
#  ZLIB_FOUND       - system has Zlib
#  ZLIB_LIBRARIES   - Link these to use Zlib
#  ZLIB_INCLUDE_DIR - Include directory for using Zlib
#  ZLIB_DEFINITIONS - Compiler switches required for using Zlib
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(LibFindMacros)

string(TOUPPER ${CMAKE_PROJECT_NAME} _UPPER_PROJECT_NAME)
set(_PROJECT_DEPENDENCY_DIR ${_UPPER_PROJECT_NAME}_DEPENDENCY_DIR)

# Check if we have cached results in case the last round was successful.
if (NOT (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY) OR NOT ZLIB_FOUND)

    set(ZLIB_LDFLAGS)
	
    find_path(ZLIB_INCLUDE_DIR zlib.h zlib/zlib.h
	    PATHS $ENV{ZLIB_DIR}/include
	          $ENV{ZLIB_DIR}
              ${${_PROJECT_DEPENDENCY_DIR}}
              ${${_PROJECT_DEPENDENCY_DIR}}/include
              ~/Library/Frameworks
              /Library/Frameworks
    )

    find_library(ZLIB_LIBRARY 
	    NAMES z zlib 
	    PATHS ${CMAKE_INSTALL_FULL_LIBDIR}
              $ENV{ZLIB_DIR}/lib
	          $ENV{ZLIB_DIR}/lib-dbg
	          $ENV{ZLIB_DIR}
              ${${_PROJECT_DEPENDENCY_DIR}}/lib
              ${${_PROJECT_DEPENDENCY_DIR}}/lib64
              ${CONAN_LIB_DIRS_LIBSBML}
              $ENV{HOME}/Library/Frameworks
              /Library/Frameworks
    )

    if (NOT WIN32)
        find_package(PkgConfig)
        pkg_check_modules(PC_ZLIB QUIET zlib)

        message(VERBOSE "${PC_ZLIB_STATIC_LDFLAGS}")

        if (PC_ZLIB_FOUND)
            set(ZLIB_DEFINITIONS ${PC_ZLIB_CFLAGS_OTHER})
            set(ZLIB_VERSION ${PC_ZLIB_VERSION} CACHE STRING "Zlib Version found" )
        endif (PC_ZLIB_FOUND)
    endif (NOT WIN32)
    
    mark_as_advanced(ZLIB_INCLUDE_DIR ZLIB_LIBRARY)

endif () # Check for cached values

# create an zlib target to link against
if(NOT TARGET ZLIB::ZLIB)
  add_library(ZLIB::ZLIB UNKNOWN IMPORTED)
  set_target_properties(ZLIB::ZLIB PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    IMPORTED_LOCATION "${ZLIB_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INCLUDE_DIR}")
endif()

set(ZLIB_PROCESS_INCLUDES ZLIB_INCLUDE_DIR)
set(ZLIB_PROCESS_LIBS ZLIB_LIBRARY)
libfind_process(ZLIB)
