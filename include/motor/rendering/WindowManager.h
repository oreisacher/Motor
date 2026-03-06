//
// Created by Orik on 12/10/2025.
//

#ifndef MOTOR_WINDOWMANAGER_H
#define MOTOR_WINDOWMANAGER_H

#include <memory>
#include <vector>

#include "Window.h"

namespace Motor {
    class WindowManager {
    public:
        WindowManager();
        ~WindowManager();

        Window* createWindow(int width, int height, const char* title, Window::RenderingBackend backend);

        Window* getWindow(int index) const;
        Window* getWindow(const char* title) const;
        std::vector<Window*> getWindows() const;

        int getWindowCount() const;

        void update();

        static Window* GetActiveWindow();

    private:
        std::vector<std::unique_ptr<Window>> windows;
        static inline int managerCount = 0;
    };
}

#endif //MOTOR_WINDOWMANAGER_H