//
// Created by Joakim on 22.03.2026.
//

#include <cmath>

#include "../Matrix/Matrix.h"
#include "../Span/Span.h"
#include "Vector.h"

namespace Objects {
    Vector::Vector() {
        components_ = new component_t[1];
        components_[0] = 0;
        cache_.n.valid = true;
        cache_.norm.valid = true;
    }

    Vector::Vector(const std::initializer_list<component_t> components) {
        cache_.n = {components.size(), true};
        if (cache_.n.is == 0) return;

        components_ = new component_t[cache_.n.is];

        size_t i = 0;
        for (const auto component : components) {
            components_[i] = component;
            i++;
        }
    }

    Vector::Vector(const Vector &other) {
        cache_ = other.cache_;

        components_ = new component_t[cache_.n.is];
        for (size_t i = 0; i < cache_.n.is; i++) {
            components_[i] = other[i];
        }
    }

    Vector::Vector(const size_t n) {
        cache_.n = {n, true};
        components_ = new component_t[cache_.n.is];

        for (size_t i = 0; i < cache_.n.is; i++) {
            components_[i] = 0;
        }
    }

    Vector::~Vector() {
        delete[] components_;
        components_ = nullptr;
    }

    norm_t Vector::norm() const { // NOLINT
        if (cache_.norm.valid) return cache_.norm.is;

        norm_t sum = 0;
        for (size_t i = 0; i < cache_.n.is; i++) sum += components_[i] * components_[i];
        cache_.norm = {std::sqrt(sum), true};

        return cache_.norm.is;
    }


    [[nodiscard]] dim_t Vector::dimension() const { return cache_.n.is; }
    [[nodiscard]] bool Vector::is_null_vec() const { return norm() == 0; }
} // Objects