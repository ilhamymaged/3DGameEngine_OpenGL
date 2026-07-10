cmake_minimum_required(VERSION 3.10)
project(3DGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Agina)
add_subdirectory(Editor)
add_subdirectory(SandBoxGame)
