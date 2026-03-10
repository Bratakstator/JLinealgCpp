//
// Created by Joakim on 3/10/2026.
//

#ifndef JLINEALGCPP_LOGGER_H
#define JLINEALGCPP_LOGGER_H

#include <iostream>
#include <filesystem>
#include <fstream>

namespace Logging {
    enum class Type {
        INFO,
        WARN,
        CRIT
    };


    inline std::string typeTstring(const Type type) {
        if (type == Type::INFO) return "[INFO] ";
        else if (type == Type::WARN) return "[WARN] ";
        else if (type == Type::CRIT) return "[CRIT] ";
        else return "[????] ";
    }

    static void setup() {
        if (const std::filesystem::path path("../dirs/logs"); !std::filesystem::exists(path)) {
            std::cout << "Creating paths: '../dirs/logs'\n";
            std::filesystem::create_directories(path);
            std::cout << "Paths created\n";
        }

        auto file = std::ofstream("../dirs/logs/log.log");
        file.close();
    }

    static void log(Type type, const std::string& text) {
        auto file = std::ofstream("../dirs/logs/log.log", std::ios::app);

        file << typeTstring(type) << text << "\n";

        file.close();
    }
} // Logging

#endif //JLINEALGCPP_LOGGER_H