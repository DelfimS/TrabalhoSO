cmake_minimum_required(VERSION 3.8)
project(TrabalhoSO)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES structs.h  servidor.c definesM.h msgstruct.h defines.h)
add_executable(TrabalhoSO ${SOURCE_FILES})
include_directories("${PROJECT_SOURCE_DIR}/dependencies")