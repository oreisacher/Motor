cmake_minimum_required(VERSION 4.0)
project(Example)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example
    src/main.cpp
    src/PlayerController.cpp
    src/OrbitCamera.cpp
)

target_include_directories(Example
    PRIVATE "include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

target_link_libraries(Example
    PUBLIC Motor
    PUBLIC glfw
)

# Pack all assets into benzin file
add_custom_target(PackageAssets ALL
    COMMAND ${ASSET_PACKER_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_dependencies(Example PackageAssets)