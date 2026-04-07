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

        cache_ = {
            {0, true},
            {0, true}
        };
    }

    Vector::Vector(const std::initializer_list<component_t> components) {
        cache_ = {
            {components.size(), true},
            {0, false}
        };
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
        cache_ = {
            {n, true},
            {0, false}
        };
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


    component_t Vector::inner_product(const Vector &other) const {
        component_t sum = 0;
        for (size_t i = 0; i < cache_.n.is; i++) sum += components_[i] * components_[i];
        return sum;
    }

    Matrix Vector::outer_product(const Vector &other) const {
        Matrix A(dimension(), other.dimension());

        for (int row = 0; row < dimension(); row++) {
            for (int col = 0; col < other.dimension(); col++) {
                A[row, col] = (*this)[row] * other[col];
            }
        }
        return A;
    }


    [[nodiscard]] dim_t Vector::dimension() const { return cache_.n.is; }
    [[nodiscard]] bool Vector::is_null_vec() const { return norm() == 0; }
} // Objects