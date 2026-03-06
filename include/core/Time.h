//
// Created by Orik on 22/10/2025.
//

#ifndef MOTOR_TIME_H
#define MOTOR_TIME_H

#include <chrono>

namespace Motor {
    class Time {
    public:
        double getTotalTime() const;
        double getDeltaTime() const;

        void update();
    private:
        double totalTime = 0.0;
        double deltaTime = 0.0;

        std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();
    };

    inline Time Time;
}

#endif //MOTOR_TIME_H