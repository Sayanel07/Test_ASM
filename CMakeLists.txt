cmake_minimum_required(VERSION 4.3)
project(Test_ASM)

set(CMAKE_CXX_STANDARD 20)

enable_language(ASM_MASM)

add_executable(Test_ASM main.cpp asm_file.asm)
