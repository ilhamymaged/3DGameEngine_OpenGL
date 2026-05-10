cmake_minimum_required(VERSION 3.10)
project(Agina LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES src/*.cpp)
list(APPEND SOURCES vendor/glad/src/glad.c vendor/stb_image/stb_image/stb_image.cpp
vendor/miniaudio/miniaudio/miniaudio.cpp)

list(APPEND SOURCES
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    src
    vendor/glad/include
    vendor/glm
    vendor/stb_image
    vendor/entt
    vendor/imgui
    vendor/imgui/backends
    vendor/spdlog
    vendor/miniaudio
)

add_subdirectory(vendor/glfw)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suite for Assimp is built" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplementary tools for Assimp are built" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "If the official samples are built" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Assimp as a static library" FORCE)

add_subdirectory(vendor/assimp)

target_link_libraries(${PROJECT_NAME} PUBLIC
    assimp
    glfw
)
