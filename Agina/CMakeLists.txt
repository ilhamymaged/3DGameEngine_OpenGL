project(Agina C CXX)

configure_file(AginaConfig.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AginaConfig.hpp")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

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

cmake_policy(SET CMP0077 NEW)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Assimp as a static library" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)     
set(INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE) 

set(ASSIMP_BUILD_AMF_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_BLEND_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_IFC_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_3DS_IMPORTER OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/assimp)

target_link_libraries(Agina PUBLIC
    assimp
    glfw
)
