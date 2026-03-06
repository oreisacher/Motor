//
// Created by Orik on 17/10/2025.
//

#ifndef MOTOR_INPUTMANAGER_H
#define MOTOR_INPUTMANAGER_H

#include <rendering/Window.h>

namespace Motor {
    class InputManager {
    public:
        void reset();

        // Add window to list of windows to which the InputManager listens for events
        void addInputWindow(Window* window);
        // TODO - Remove window function

        bool isKeyPressed(Window::Key key);

        std::pair<int, int> getMousePosition() const;
        std::pair<double, double> getScrollDelta() const;

    private:
        std::unordered_map<Window::Key, bool> keys;
        std::unordered_map<Window::MouseButton, bool> mouseButtons;

        std::pair<int, int> mousePosition { 0, 0 };
        std::pair<double, double> mouseScrollDelta { 0.0, 0.0 };
    };

    // Global access to InputManager
    inline InputManager Input;
}

#endif //MOTOR_INPUTMANAGER_H