# Locate libsbml
# This module defines:
# LIBSBML_INCLUDE_DIR, where to find the headers
#
# LIBSBML_LIBRARY, LIBSBML_LIBRARY_DEBUG
# LIBSBML_FOUND, LIBSBML_LIBRARY_NAME

include(LibFindMacros)

set(_SBML_HINTS
  ${SBML_ROOT}/include
  ${SBML_ROOT}/lib
  $ENV{SBML_ROOT}
  $ENV{HOME}/usr/include
  $ENV{HOME}/usr/lib
  /usr/local/include
  /usr/local/lib
  /usr/include
  /usr/lib
)

libfind_pkg_check_modules(LIBSBML_PKGCONF sbml)

find_path(LIBSBML_INCLUDE_DIR
  NAMES sbml/SBMLTypes.h
  PATH_SUFFIXES sbml
  HINTS ${_SBML_HINTS}
  PATHS ${LIBSBML_PKGCONF_INCLUDE_DIRS}
)

if (LIBSBML_INCLUDE_DIR AND EXISTS "${LIBSBML_INCLUDE_DIR}/sbml/common/libsbml-version.h")
  libfind_version_header(LIBSBML "sbml/common/libsbml-version.h" "LIBSBML_DOTTED_VERSION")
endif()

find_library(LIBSBML_LIBRARY
  NAMES sbml
  HINTS ${_sbml_hints}
  PATH ${LIBSBML_PKGCONF_LIBRARY_DIRS}
)

set(LIBSBML_PROCESS_INCLUDES LIBSBML_INCLUDE_DIR)
set(LIBSBML_PROCESS_LIBS LIBSBML_LIBRARY)
libfind_process(LIBSBML)
