//
// Created by Orik on 22/10/2025.
//

#include <motor/core/Time.h>

using namespace Motor;

double Time::getTotalTime() const { return totalTime; }
double Time::getDeltaTime() const { return deltaTime; }

void Time::update() {
    auto now = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<double>(now - lastTime).count();
    totalTime += deltaTime;
    lastTime = now;
}
