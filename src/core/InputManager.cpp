//
// Created by Orik on 17/10/2025.
//


#include <motor/core/InputManager.h>

using namespace Motor;

void InputManager::reset() {
    mouseScrollDelta = { 0.0, 0.0 };
}

void InputManager::addInputWindow(Window *window) {
    // Mouse Position
    window->addMouseCallback([&](Window *window, double x, double y) {
        mousePosition = { static_cast<int>(x), static_cast<int>(y) };
    });

    // Mouse Scroll Delta
    window->addMouseScrollCallback([&](Window *window, double x, double y) {
        mouseScrollDelta = { static_cast<int>(x), static_cast<int>(y) };
    });

    // Mouse Button
    window->addMouseButtonCallback([&](Window *window, Window::MouseButton button, Window::Action action, int mods) {
        mouseButtons[button] = action == Window::Action::PRESS;
    });

    // Key
    window->addKeyCallback([&](Window *window, Window::Key key, Window::Action action, int mods) {
        keys[key] = (action == Window::Action::PRESS || action == Window::Action::HOLD);
    });
}

bool InputManager::isKeyPressed(Window::Key key) {
    auto it = keys.find(key);
    return it != keys.end() && it->second;
}

std::pair<int, int> InputManager::getMousePosition() const { return mousePosition; }
std::pair<double, double> InputManager::getScrollDelta() const { return mouseScrollDelta; }
