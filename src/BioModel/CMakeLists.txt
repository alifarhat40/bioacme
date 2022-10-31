cmake_minimum_required(VERSION 3.8)

project(BioModel)

find_package(LIBSBML REQUIRED)

include_directories("${LIBSBML_INCLUDE_DIR}")

add_library(${PROJECT_NAME}
    HashTable.cpp
    HashTable.h
    InitialConds.cpp
    InitialConds.h
    MatrixElement.cpp
    MatrixElement.h
    Network.cpp
    Network.h
    Queue.cpp
    Queue.h
    SLinkedList.cpp
    SLinkedList.h
    SparseMatrix.cpp
    SparseMatrix.h
    Stack.cpp
    Stack.h
    State.cpp
    State.h
    StateSpace.cpp
    StateSpace.h
    StateType.h
    Stoichiometry.cpp
    Stoichiometry.h
)

target_link_libraries(${PROJECT_NAME} ${LIBSBML_LIBRARY})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
