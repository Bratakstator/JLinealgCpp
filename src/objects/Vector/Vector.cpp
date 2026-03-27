//
// Created by Joakim on 22.03.2026.
//

#include "../Matrix/Matrix.h"
#include "../Span/Span.h"
#include "Vector.h"

namespace Objects {
    Vector::Vector() { components_ = new component_t[1]; }

    Vector::Vector(const std::initializer_list<component_t> components) {
        n_ = components.size();
        if (n_ == 0) return;

        components_ = new component_t[n_];

        size_t i = 0;
        for (const auto component : components) {
            components_[i] = component;
            i++;
        }

        if (norm() == 0) {
            n_ = 0;
            delete components_;
            components_ = new component_t[1];
            changed_ = true;
        }
        else isNullPtr = false;
    }

    Vector::Vector(const Vector &other) {
        n_ = other.n_;
        norm_ = other.norm_;
        changed_ = other.changed_;
        isNullPtr = other.isNullPtr;

        components_ = new component_t[n_];
        for (size_t i = 0; i < n_; i++) {
            components_[i] = other[i];
        }
    }

    Vector::Vector(const size_t n) {
        n_ = n;
        components_ = new component_t[n_];
        isNullPtr = false;

        for (size_t i = 0; i < n_; i++) {
            components_[i] = 0;
        }
    }

    Vector::~Vector() {
        delete[] components_;
        components_ = nullptr;
    }

    norm_t Vector::norm() { // NOLINT
        if (!changed_) return norm_;

        norm_t sum = 0;
        for (size_t i = 0; i < n_; i++) sum += components_[i] * components_[i];
        norm_ = std::sqrt(sum);
        changed_ = false;

        return norm_;
    }


    [[nodiscard]] dim_t Vector::dimension() const { return n_; }
    [[nodiscard]] bool Vector::is_null_vec() const { return isNullPtr; }
} // Objects