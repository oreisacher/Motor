//
// Motor Engine Example
//

#include <random>
#include <utils/Logger.h>
#include <assets/AssetManager.h>

#include "OrbitCamera.h"
#include "PlayerController.h"
#include "components/Animator.h"
#include "core/InputManager.h"
#include "core/ModelNode.h"
#include "core/Time.h"
#include "rendering/OpenGLRenderer.h"
#include "rendering/WindowManager.h"

using namespace Motor;

int main() {
    Logger::activateLevel(Logger::Info);

    // -----------------------
    // --- Window Creation ---
    // -----------------------
    WindowManager windowManager;

    auto mainWindow = windowManager.createWindow(1000, 1000, "Main", Window::RenderingBackend::OPENGL);
    mainWindow->setCursorMode(Window::CursorMode::CURSOR_HIDDEN);
    mainWindow->addKeyCallback([&](Window* window, Window::Key key, Window::Action action, int mods) {
        if (key == Window::Key::Escape)
            window->close();
        if (key == Window::Key::F1)
            window->setCursorMode(Window::CursorMode::CURSOR_NORMAL);
        if (key == Window::Key::F2)
            window->setCursorMode(Window::CursorMode::CURSOR_HIDDEN);
    });

    Input.addInputWindow(mainWindow);

    // ----------------------
    // --- Renderer Setup ---
    // ----------------------
    OpenGLRenderer renderer;
    renderer.init();

    mainWindow->addResizeCallback([&](Window* window, int width, int height) {
        renderer.updateWindowSize(width, height);
    });

    // -------------------
    // --- Setup Scene ---
    // -------------------
    auto assetManager = AssetManager::getInstance();
    assetManager->setResourceFile("resources.benzin");

    Scene* world = new Scene("Main");
    auto worldRoot = world->getRoot();

    // Skybox
    auto skyboxTexture = assetManager->load<Texture>("textures/sunset.png");
    skyboxTexture->setTextureType(Texture::Type::DIFFUSE);
    world->setSkyboxTexture(skyboxTexture);

    // Ground
    auto groundModel = assetManager->load<Model>("models/plane.gltf");
    auto groundInstance = groundModel->instantiate();

    auto ground = new ModelNode("Ground");
    ground->setModelInstance(groundInstance.get());
    ground->setPosition({0.0, 0.0f, 0.0f});
    ground->setRotation({0.9, 0.0, 0.0});
    ground->setScale({50.0f, 1.0f, 50.0});
    worldRoot->addChild(ground);

    auto grass = assetManager->load<Material>("grass.material");
    grass->setRoughness(0.6f);
    grass->setTextureTiling(50.0f);
    groundInstance->setMaterialForMesh(0, grass);

    // Rock
    auto rockModel = assetManager->load<Model>("models/rock/namaqualand_cliff_02_4k.gltf");
    auto rockModelInstance = rockModel->instantiate();

    auto rockNode = new ModelNode("Rock");
    rockNode->setModelInstance(rockModelInstance.get());
    rockNode->setPosition({0.0, 0.0, -3.0});
    rockNode->setScale({0.5, 0.5, 0.5});
    worldRoot->addChild(rockNode);

    // Boulder
    auto boulderModel = assetManager->load<Model>("models/boulder/boulder_01_4k.gltf");
    auto boulderInstance = boulderModel->instantiate();
    boulderInstance->getMaterialForMesh(0)->setRoughness(0.9f);

    auto boulderNode1 = new ModelNode("Boulder 1");
    boulderNode1->setModelInstance(boulderInstance.get());
    boulderNode1->setPosition({5.0, 0.0, 9.0});
    boulderNode1->setScale({4, 4, 4});
    worldRoot->addChild(boulderNode1);

    auto boulderNode2 = new ModelNode("Boulder 2");
    boulderNode2->setModelInstance(boulderInstance.get());
    boulderNode2->setPosition({-5.0, 0.0, 9.0});
    boulderNode2->setScale({4, 4, 4});
    boulderNode2->setRotation({0.0f, 45.0f, 0.0f});
    worldRoot->addChild(boulderNode2);

    // Table
    auto tableModel = assetManager->load<Model>("models/table/round_wooden_table_02_4k.gltf");
    auto tableInstance = tableModel->instantiate();
    tableInstance->getMaterialForMesh(0)->setRoughness(0.8f);

    auto tableNode = new ModelNode("Table");
    tableNode->setModelInstance(tableInstance.get());
    tableNode->setPosition({ 0.0f, 0.0f, 0.0f });
    worldRoot->addChild(tableNode);

    // Camera
    auto cameraModel = assetManager->load<Model>("models/camera/Camera_01_4k.gltf");
    auto cameraInstance = cameraModel->instantiate();

    auto cameraNode = new ModelNode("Camera");
    cameraNode->setModelInstance(cameraInstance.get());
    cameraNode->setScale({2.0f, 2.0f, 2.0f});
    cameraNode->setPosition({0.1f, 0.748f, -0.1f});
    worldRoot->addChild(cameraNode);

    // Apples
    auto appleModel = assetManager->load<Model>("models/apple/food_apple_01_4k.gltf");
    auto appleInstance = appleModel->instantiate();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> sizeDist(1.0f, 3.0f);

    for (int i = 0 ; i < 1000 ; i++) {
        auto appleNode = new ModelNode(std::format("Apple{}", i));
        appleNode->setModelInstance(appleInstance.get());
        appleNode->setPosition({ dist(gen), 0.0f, dist(gen) });
        float size = sizeDist(gen);
        appleNode->setScale({ size, size, size });
        worldRoot->addChild(appleNode);
    }

    // Character
    auto characterModel = assetManager->load<Model>("models/character/character.gltf");
    auto characterInstance = characterModel->instantiate();
    characterInstance->getMaterialForMesh(0)->setRoughness(0.9f);

    auto characterNode = new ModelNode("Character");
    characterNode->setModelInstance(characterInstance.get());
    characterNode->setScale({0.5f, 0.5f, 0.5f});
    characterNode->setPosition({ 0.0f, 0.0f, 5.0f});
    characterNode->setRotation({ 0.0f, 180.0f, 0.0f});
    worldRoot->addChild(characterNode);

    auto cameraTargetNode = new SceneNode("CameraTarget");
    cameraTargetNode->setPosition({ 0.0f, 0.017f, 0.0f });
    characterNode->addChild(cameraTargetNode);

    /*
     * This character could be animated using an Animator component.
     * You could load animations such as idle, walk, run, attack, etc.
     * Models with such animations can be found on website such as Mixamo.
     *

    characterNode->addComponent(std::make_unique<Animator>());
    auto animator = characterNode->getComponent<Animator>();
    animator->setAnimation("idle");
    animator->setState(Animator::PLAYING);
    */

    // Light
    auto spotLight = new LightNode("PointLight", LightNode::Type::SPOT);
    spotLight->setOuterAngle(80.0f);
    spotLight->setInnerAngle(45.0f);
    spotLight->setPosition({0.0f, 2.0f, 2.0f});
    spotLight->setRotation({-45.0f, 0.0f, 0.0f});
    worldRoot->addChild(spotLight);

    // Third person character controller
    auto camera = new CameraNode("MainCamera", 90.0f, mainWindow->getAspect(), 0.1f, 100.0f);
    camera->setPosition({ 0.0f, 0.0f, 2.0f});
    camera->setNear(0.01f);
    worldRoot->addChild(camera);

    camera->addComponent(std::make_unique<OrbitCamera>());
    camera->getComponent<OrbitCamera>()->target = cameraTargetNode;

    characterNode->addComponent(std::make_unique<PlayerController>());
    characterNode->getComponent<PlayerController>()->camera = camera;

    // Notice the above note about animations
    // characterNode->getComponent<PlayerController>()->animator = animator;

    mainWindow->addResizeCallback([&](Window* window, int width, int height) {
        camera->setAspect((float)width / (float)height);
    });

    // ------------------
    // --- World Init ---
    // ------------------
    world->init();
    world->updateGlobalTransforms();
    world->printTree();

    // -----------------
    // --- MAIN LOOP ---
    // -----------------
    while (windowManager.getWindowCount() > 0) {
        // Update components of each scene node
        world->update(Time.getDeltaTime());

        // Update global transforms of each scene node
        world->updateGlobalTransforms();

        // Render the scene to the screen
        renderer.render(world);

        // End of frame
        Input.reset();
        mainWindow->swapBuffers();
        windowManager.update();
        Time.update();
    }

    renderer.shutdown();
}
