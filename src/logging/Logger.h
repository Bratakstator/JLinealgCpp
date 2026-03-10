//
// Created by Joakim on 3/10/2026.
//

#ifndef JLINEALGCPP_LOGGER_H
#define JLINEALGCPP_LOGGER_H

namespace Logging {
    enum class Type {
        INFO,
        WARN,
        CRIT
    };

    static std::string typeTstring(Type type) {
        if (type == Type::INFO) return "[INFO]";
        else if (type == Type::WARN) return "[WARN]";
        else if (type == Type::CRIT) return "[CRIT]";
        else return "[????]";
    }
} // Logging

#endif //JLINEALGCPP_LOGGER_H