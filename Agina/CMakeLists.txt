cmake_minimum_required(VERSION 3.10)
project(Agina LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES src/*.cpp)
list(APPEND SOURCES vendor/glad/src/glad.c vendor/stb_image/stb_image/stb_image.cpp)

list(APPEND SOURCES
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    vendor/glfw
    vendor/glad/include
    src
    vendor/glm
    vendor/stb_image
    vendor/entt
    vendor/imgui
    vendor/imgui/backends
    vendor/assimp/include
    vendor/assimp/build/include
)

link_directories()

target_link_libraries(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/Agina/vendor/glfw/glfw3.lib"
    opengl32 
    "${CMAKE_SOURCE_DIR}/Agina/vendor/assimp/build/lib/Release/assimp-vc143-mt.lib"
)
