cmake_minimum_required(VERSION 3.10)
project(PhantomStresser LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_executable(phantom_stresser attackpanel.c)

target_compile_options(phantom_stresser PRIVATE -Wall -Wextra -O2)

target_link_libraries(phantom_stresser PRIVATE raylib m pthread)

if(UNIX AND NOT APPLE)
    target_link_libraries(phantom_stresser PRIVATE GL dl X11)
endif()

if(APPLE)
    target_link_libraries(phantom_stresser PRIVATE
        "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

if(WIN32)
    target_link_libraries(phantom_stresser PRIVATE opengl32 gdi32 winmm)
endif()

install(TARGETS phantom_stresser DESTINATION bin)

message(STATUS "Project: Phantom Stresser")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Binary: phantom_stresser")