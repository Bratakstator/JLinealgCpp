//
// Created by Joakim on 22.03.2026.
//

#include "Vector.h"

namespace Objects {
    Vector::Vector() { elements_ = new double[1]; }

    Vector::Vector(const std::initializer_list<double> elements) {
        n_ = elements.size();
        if (n_ == 0) return;

        elements_ = new double[n_];

        int i = 0;
        for (const auto element : elements) {
            elements_[i] = element;
            i++;
        }

        if (norm() == 0) {
            n_ = 0;
            delete elements_;
            elements_ = new double[1];
            changed_ = true;
        }
        else isNullPtr = false;
    }

    Vector::Vector(const Vector &other) {
        n_ = other.n_;
        norm_ = other.norm_;
        changed_ = other.changed_;
        isNullPtr = other.isNullPtr;

        elements_ = new double[n_];
        for (int i = 0; i < n_; i++) {
            elements_[i] = other[i];
        }
    }

    Vector::Vector(const int n) {
        n_ = n;
        elements_ = new double[n_];
        isNullPtr = false;

        for (int i = 0; i < n_; i++) {
            elements_[i] = 0;
        }
    }

    Vector::~Vector() {
        delete[] elements_;
        elements_ = nullptr;
    }

    double Vector::norm() { // NOLINT
        if (!changed_) return norm_;

        double sum = 0;
        for (int i = 0; i < n_; i++) sum += elements_[i] * elements_[i];
        norm_ = std::sqrt(sum);
        changed_ = false;

        return norm_;
    }


    [[nodiscard]] int Vector::size() const { return n_; }
    [[nodiscard]] bool Vector::is_null_vec() const { return isNullPtr; }
} // Objects