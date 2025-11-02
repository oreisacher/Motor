cmake_minimum_required(VERSION 3.26)
project(Motor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Pull git submodules
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    message(STATUS "Submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "${GIT_SUBMOD_RESULT}")
    endif()
endif()

# Before add_subdirectory() for each library
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)          # Force static
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)     # GLFW static
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)   # Assimp static
set(ASSIMP_BUILD_STATIC_LIB ON CACHE BOOL "" FORCE)   # Assimp static
set(ZSTD_BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)     # Zstd static
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)     # Zstd static
set(ASSIMP_BUILD_MINIZIP ON CACHE BOOL "" FORCE)

add_subdirectory("external/assimp")
add_subdirectory("external/glfw")
add_subdirectory("external/glm")
add_subdirectory("external/json")
add_subdirectory("external/zstd/build/cmake")

add_subdirectory("src")

# Example
add_subdirectory("example")