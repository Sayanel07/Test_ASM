cmake_minimum_required(VERSION 4.3)

set(CMAKE_CXX_STANDARD 20)

project(Test_ASM)

enable_language(ASM_MASM)

file(GLOB SOURCES
        source/*.cpp
        header/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} asm_file.asm)
