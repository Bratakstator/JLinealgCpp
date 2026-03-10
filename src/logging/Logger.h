//
// Created by Joakim on 3/10/2026.
//

#ifndef JLINEALGCPP_LOGGER_H
#define JLINEALGCPP_LOGGER_H

#include <filesystem>
#include <fstream>

namespace Logging {
    enum class Type {
        INFO,
        WARN,
        CRIT
    };


    static std::ofstream file_;


    static std::string typeTstring(const Type type) {
        if (type == Type::INFO) return "[INFO]";
        else if (type == Type::WARN) return "[WARN]";
        else if (type == Type::CRIT) return "[CRIT]";
        else return "[????]";
    }

    static void setup() {
        if (const std::filesystem::path path("../dirs/logs"); !std::filesystem::exists(path)) std::filesystem::create_directories(path);
        file_ = std::ofstream("../dirs/logs/log_verbose.log");
    }
} // Logging

#endif //JLINEALGCPP_LOGGER_H