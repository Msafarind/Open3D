include(ExternalProject)

set(OPENBLAS_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/openblas)

ExternalProject_Add(
    ext_openblas
    PREFIX openblas
    GIT_REPOSITORY https://github.com/xianyi/OpenBLAS.git
    GIT_TAG v0.3.10
    # INSTALL_COMMAND "" # Install command not compatible with powershell
    CMAKE_ARGS
        -DMSVC_STATIC_CRT=${STATIC_WINDOWS_RUNTIME}
        -DCMAKE_INSTALL_PREFIX=${OPENBLAS_INSTALL_PREFIX}
)

set(OPENBLAS_INCLUDE_DIR "${OPENBLAS_INSTALL_PREFIX}/include/openblas")
set(OPENBLAS_LIB_DIR "${OPENBLAS_INSTALL_PREFIX}/lib")
set(OPENBLAS_LIBRARIES openblas)

message(STATUS "OPENBLAS_INCLUDE_DIR: ${OPENBLAS_INCLUDE_DIR}")
message(STATUS "OPENBLAS_LIB_DIR ${OPENBLAS_LIB_DIR}")
message(STATUS "OPENBLAS_LIBRARIES: ${OPENBLAS_LIBRARIES}")