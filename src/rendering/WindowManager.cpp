//
// Created by Orik on 12/10/2025.
//

#include "rendering/WindowManager.h"
#include <format>
#include <utils/Logger.h>
#include <GLFW/glfw3.h>

using namespace Motor;

WindowManager::WindowManager() {
    glfwSetErrorCallback([](int error, const char* description) {
        LOG_ERROR(std::format("Error [{}]: {}", error, description))
    });

    // Initialize GLFW
    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW")
        return;
    }

    managerCount++;
}

WindowManager::~WindowManager() {
    managerCount--;

    if (managerCount == 0) {
        glfwTerminate();
    }
}

Window* WindowManager::createWindow(int width, int height, const char *title, Window::RenderingBackend backend) {
    auto window = new Window(width, height, title, backend);
    windows.emplace_back(window);
    return window;
}

Window* WindowManager::getWindow(int index) const {
    return windows[index].get();
}

Window* WindowManager::getWindow(const char *title) const {
    for (auto &window : windows) {
        if (window->getTitle() == title)
            return window.get();
    }

    return nullptr;
}

std::vector<Window*> WindowManager::getWindows() const {
    std::vector<Window*> results;

    for (auto &window : windows)
        results.emplace_back(window.get());

    return results;
}

int WindowManager::getWindowCount() const { return windows.size(); }

void WindowManager::update() {
    glfwPollEvents();

    std::erase_if(windows,
    [](const std::unique_ptr<Window>& w) {
          return w->shouldClose();
    });
}

Window* WindowManager::GetActiveWindow() {
    return static_cast<Window*>(glfwGetWindowUserPointer(glfwGetCurrentContext()));
}
