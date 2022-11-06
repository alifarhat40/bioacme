include(LibFindMacros)
include(GNUInstallDirs)

string(TOUPPER ${CMAKE_PROJECT_NAME} _UPPER_PROJECT_NAME)
set(_PROJECT_DEPENDENCY_DIR ${_UPPER_PROJECT_NAME}_DEPENDENCY_DIR)

find_path(BZ2_INCLUDE_DIR
  NAMES bzlib.h bzip2/bzlib.h
  PATHS $ENV{BZ2_DIR}/include
        $ENV{BZ2_DIR}
        ${${_PROJECT_DEPENDENCY_DIR}}/include
        ${${_PROJECT_DEPENDENCY_DIR}}
        ${BZ2_PKGCONF_INCLUDE_DIRS}
)

if (BZ2_INCLUDE_DIR)
    file(STRINGS "${BZ2_INCLUDE_DIR}/bzlib.h" BZLIB_H REGEX "bzip2/libbzip2 version [0-9]+\\.[^ ]+ of [0-9]+ ")
    string(REGEX REPLACE ".* bzip2/libbzip2 version ([0-9]+\\.[^ ]+) of [0-9]+ .*" "\\1" BZ2_VERSION "${BZLIB_H}")
endif ()

find_library(BZ2_LIBRARY
  NAMES bzip2 bz2
  PATHS $ENV{BZ2_DIR}/lib
        $ENV{BZ2_DIR}
        ${${_PROJECT_DEPENDENCY_DIR}}/lib
        ${${_PROJECT_DEPENDENCY_DIR}}
        ${LIBZ_PKGCONF_LIBRARY_DIRS}
)

include(CheckLibraryExists)

if (BZ2_LIBRARY)
    # make sure that we have a valid bzip2 library
    file(TO_CMAKE_PATH "${BZ2_LIBRARY}" BZ2_CMAKE_PATH)
    CHECK_LIBRARY_EXISTS("${BZ2_CMAKE_PATH}" "BZ2_bzCompressInit" "" BZ2_FOUND_SYMBOL)
    if(NOT BZ2_FOUND_SYMBOL)
        # this is odd, but on windows this check always fails! must be a
        # bug in the current cmake version so for now only issue this
        # warning on linux
        if(UNIX)
            message(WARNING
"The chosen bz2 library does not appear to be valid because it is
missing some required symbols. Please check that ${BZ2_LIBRARY}
is the bzip2 library. For details about the error, please see
${LIBSBML_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log")
        endif()
    endif()
    if(NOT EXISTS "${BZ2_INCLUDE_DIR}/bzlib.h")
        message(FATAL_ERROR
"The include directory specified for the bz2 library does not
appear to be valid.  It should contain the file bzlib.h, but
it does not.")
    endif()
endif()

# create an bz2 target to link against
if(NOT TARGET BZ2::BZ2)
  add_library(BZ2::BZ2 UNKNOWN IMPORTED)
  set_target_properties(BZ2::BZ2 PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    IMPORTED_LOCATION "${BZ2_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${BZ2_INCLUDE_DIR}"
    )
endif()

set(BZ2_PROCESS_INCLUDES BZ2_INCLUDE_DIR)
set(BZ2_PROCESS_LIBS BZ2_LIBRARY)
libfind_process(BZ2)
