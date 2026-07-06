file(GLOB_RECURSE EDITOR_SOURCES src/*.cpp)

add_executable(Editor ${EDITOR_SOURCES})

target_link_libraries(Editor PRIVATE Agina)

target_include_directories(Editor PRIVATE
	src
)

if (MSVC)
    target_compile_options(Editor PRIVATE /utf-8)
endif()

