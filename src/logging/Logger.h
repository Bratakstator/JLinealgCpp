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

    enum class Class {
        NONE,
        ComponentPPProxy,
        ComponentPProxy,
        ComponentProxy,
        VectorProxy,
        AugmentedMatrix,
        AugmentedMatrixVectorPair,
        Matrix,
        Span,
        Vector
    };


    inline std::string typeTstring(const Type type) {
        if (type == Type::INFO)         return "[..INFO..]";
        else if (type == Type::WARN)    return "[WARNING.]";
        else if (type == Type::CRIT)    return "[CRITICAL]";
        else                            return "[????????]";
    }

    inline std::string classTstring(const Class clss) {
        if (clss == Class::NONE)                            return "[.........................]";
        else if (clss == Class::ComponentPPProxy)           return "[.....ComponentPPProxy....]";
        else if (clss == Class::ComponentPProxy)            return "[.....ComponentPProxy.....]";
        else if (clss == Class::ComponentProxy)             return "[.....ComponentProxy......]";
        else if (clss == Class::VectorProxy)                return "[.......VectorProxy.......]";
        else if (clss == Class::AugmentedMatrix)            return "[.....AugmentedMatrix.....]";
        else if (clss == Class::AugmentedMatrixVectorPair)  return "[AugmentedMatrixVectorPair]";
        else if (clss == Class::Matrix)                     return "[.........Matrix..........]";
        else if (clss == Class::Span)                       return "[..........Span...........]";
        else if (clss == Class::Vector)                     return "[.........Vector..........]";
        else                                                return "[?????????????????????????]";
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

    static void log(const Type type, const Class clss, const std::string& text) {
        auto file = std::ofstream("../dirs/logs/log.log", std::ios::app);
        file << typeTstring(type) << classTstring(clss) << " " << text << "\n";
        file.close();
    }
} // Logging

#endif //JLINEALGCPP_LOGGER_H