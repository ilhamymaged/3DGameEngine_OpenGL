cmake_minimum_required(VERSION 3.10)
project(3DGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Agina)
add_subdirectory(Editor)
add_subdirectory(SandBoxGame)

if (MSVC)
    # Ensure the debug information format supports Edit and Continue (/ZI)
    # Turn off optimizations (/Od) and enable incremental linking
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /ZI /Ob0 /Od /RTC1")
    
    # Enable incremental linking for the linker stage
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/INCREMENTAL")
endif()

