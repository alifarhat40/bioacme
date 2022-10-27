cmake_minimum_required(VERSION 3.8)

project(mxexp)

enable_language(Fortran)

# set(LIBSBML_ROOT "C:\\Program Files (x86)\\libsbml")

# add_library(libsbml STATIC IMPORTED)
# set_target_properties(libsbml PROPERTIES IMPORTED_LOCATION ${LIBSBML_ROOT}/lib/libsbml-static.lib)
# set_target_properties(libsbml PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${LIBSBML_ROOT}/include)
#SET(CMAKE_EXE_LINKER_FLAGS  "/FORCE")
# set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -w -fallow-argument-mismatch ")

# add_library(libsbml SHARED IMPORTED)
   
# set_target_properties(libsbml PROPERTIES
#     IMPORTED_LOCATION "${LIBSBML_ROOT}/bin/libsbml.dll"
#     IMPORTED_IMPLIB "${LIBSBML_ROOT}/lib/libsbml.lib"
#     INTERFACE_INCLUDE_DIRECTORIES "${LIBSBML_ROOT}/include"
# )

# if (MSVC)
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:5000000000")
# else ()
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000")
# endif()

# target_link_libraries(${PROJECT_NAME} ${LIBSBML_ROOT}/bin/ibsbml.dll)     
#include_directories(${LIBSBML_ROOT}/include)

add_executable(${PROJECT_NAME} 
    blas.f
    lapack.f
    clock.f
    dgpadm.f 
    dgexpv.f
    mataid.f
    mxexp.f
)
    
# target_link_libraries(${PROJECT_NAME} libsbml)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})