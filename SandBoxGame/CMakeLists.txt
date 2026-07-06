file(GLOB_RECURSE SANDBOX_SOURCES src/*.cpp)
add_executable(SandBoxGame ${SANDBOX_SOURCES})

target_link_libraries(SandBoxGame PRIVATE Agina)

if (MSVC)
    target_compile_options(SandBoxGame PRIVATE /utf-8)
endif() 