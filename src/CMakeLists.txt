include(CMakePackageConfigHelpers)

project(Motor)

# Compile library
add_library(Motor STATIC
        Motor.cpp
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
        assets/Animation.cpp
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
)

target_include_directories(Motor
        PUBLIC ${CMAKE_SOURCE_DIR}/include
        PUBLIC ${CMAKE_SOURCE_DIR}/external/glad/include
        PUBLIC ${CMAKE_SOURCE_DIR}/external/glfw/include
        PUBLIC ${CMAKE_SOURCE_DIR}/external/assimp/include
        PUBLIC ${CMAKE_SOURCE_DIR}/external/glm
        PUBLIC ${CMAKE_SOURCE_DIR}/external/json/include
        PUBLIC ${CMAKE_SOURCE_DIR}/external/stb
        PUBLIC ${CMAKE_SOURCE_DIR}/external/zstd/lib
        PUBLIC ${CMAKE_SOURCE_DIR}/external/assimp/contrib/zlib
)

target_compile_definitions(Motor PUBLIC GLFW_STATIC)
target_compile_definitions(Motor PUBLIC ASSIMP_BUILD_STATIC_LIB)

target_link_libraries(Motor
        PUBLIC assimp
        PUBLIC glfw
        PUBLIC libzstd_static
        PUBLIC opengl32
)

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