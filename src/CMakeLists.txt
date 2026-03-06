include(CMakePackageConfigHelpers)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib)

# Compile library
add_library(Motor STATIC
    core/SceneNode.cpp
    core/Scene.cpp
    assets/Asset.cpp
    assets/Texture.cpp
    utils/Logger.cpp
    assets/loaders/TextureLoader.cpp
    assets/Shader.cpp
    assets/Material.cpp
    core/ShaderInstance.cpp
    assets/loaders/ShaderLoader.cpp
    assets/loaders/ShaderSourceLoader.cpp
    assets/ShaderSource.cpp
    assets/AssetManager.cpp
    assets/loaders/MaterialLoader.cpp
    assets/BenzinLoader.cpp
    assets/Mesh.cpp
    assets/Model.cpp
    assets/loaders/ModelLoader.cpp
    assets/BenzinAssimpAdapater.cpp
    core/ModelNode.cpp
    rendering/Window.cpp
    rendering/WindowManager.cpp
    core/CameraNode.cpp
    rendering/OpenGLRenderer.cpp
    rendering/GPUShader.cpp
    rendering/GPUTexture.cpp
    rendering/GPUMesh.cpp
    rendering/GPUResourceManager.cpp
    core/InputManager.cpp
    assets/Animation.cpp
    components/Animator.cpp
    components/FreeCam.cpp
    components/Component.cpp
    core/Time.cpp
    core/LightNode.cpp
    rendering/renderpass/ForwardRenderPass.cpp
    rendering/renderpass/ShadowPass.cpp
    assets/ModelInstance.cpp
    rendering/renderpass/SkyboxPass.cpp
    rendering/renderpass/CompositionPass.cpp
    rendering/renderpass/ShaderPass.cpp
    rendering/renderpass/FullscreenPass.cpp
    rendering/pipeline/RenderPipeline.cpp
    rendering/pipeline/DefaultPipeline.cpp
)

target_include_directories(Motor
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/stb
)

target_compile_definitions(Motor PUBLIC GLFW_STATIC)
target_compile_definitions(Motor PUBLIC ASSIMP_BUILD_STATIC_LIB)

target_link_libraries(Motor
    PRIVATE assimp
    PUBLIC glfw
    PUBLIC libzstd_static
    PUBLIC glm
    PUBLIC nlohmann_json
)

if(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Motor PUBLIC OpenGL::GL)
elseif(WIN32)
    target_link_libraries(Motor PUBLIC opengl32)
endif()

# Build Asset Packer tool
add_executable(AssetPacker
    tools/BenzinPacker.cpp
)

target_include_directories(AssetPacker
    PUBLIC ${CMAKE_SOURCE_DIR}/../external/zstd/lib
)

target_link_libraries(AssetPacker
    libzstd_static
)

add_dependencies(Motor AssetPacker)

set(ASSET_PACKER_EXECUTABLE $<TARGET_FILE:AssetPacker> CACHE FILEPATH "Path to AssetPacker executable")
message(STATUS "AssetPacker path: ${ASSET_PACKER_EXECUTABLE}")