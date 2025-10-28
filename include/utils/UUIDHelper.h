//
// Created by Orik on 21/09/2025.
//

#ifndef MOTOR_UUIDHELPER_H
#define MOTOR_UUIDHELPER_H

#include <sstream>
#include <iostream>
#include <iomanip>
#include <random>

namespace Motor {
    class UUIDHelper {
    public:
        static std::string generateUUID() {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<uint32_t> dist;

            std::stringstream ss;
            ss << std::hex << std::setfill('0');
            for (int i = 0 ; i < 4 ; ++i) {
                ss << std::setw(8) << dist(gen);
                if (i != 3) ss << "-";
            }

            return ss.str();
        }
    };
}

#endif //MOTOR_UUIDHELPER_H