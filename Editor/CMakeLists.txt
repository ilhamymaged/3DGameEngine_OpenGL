cmake_minimum_required(VERSION 3.10)
project(Editor LANGUAGES CXX C)
file(GLOB_RECURSE EDITOR_SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${EDITOR_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Agina)
target_include_directories(${PROJECT_NAME} PRIVATE
	src
)