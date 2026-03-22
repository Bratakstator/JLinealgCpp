//
// Created by Joakim on 22.03.2026.
//

#include "Vector.h"

namespace Objects {
    Vector& Vector::operator=(const Vector &other) {
        if (this == &other) return *this;
        n_ = other.n_;
        norm_ = other.norm_;
        changed_ = other.changed_;
        isNullPtr = other.isNullPtr;

        elements_ = new double[n_];

        for (int i = 0; i < n_; i++) {
            elements_[i] = other.elements_[i];
        }
        return *this;
    }


    DoubleProxy Vector::operator[](int p) { // NOLINT
        if (n_ == 0) return {*elements_, changed_, isNullPtr};
        if (p >= n_) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << p << ", but size of Vector is: " << n_ << "\n";
            throw std::out_of_range(err.str());
        }
        return {elements_[p], changed_, isNullPtr};
    }
    double Vector::operator[](const int p) const {
        if (n_ == 0) return 0;
        if (p >= n_) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << p << ", but size of Vector is: " << n_ << "\n";
            throw std::out_of_range(err.str());
        }
        return elements_[p];
    }

    Vector& Vector::operator+=(const Vector &other) {
        if (isNullPtr && !other.is_null_vec()) {
            n_ = other.size();
            delete[] elements_;
            elements_ = new double[n_];

            for (int i = 0; i < n_; i++) elements_[i] = other[i];
            isNullPtr = false;
            changed_ = true;

            return *this;
        }
        if (other.is_null_vec()) return *this;

        for (int i = 0; i < n_; i++) elements_[i] += other[i];
        changed_ = true;

        return *this;
    }

    Vector& Vector::operator-=(const Vector &other) {
        if (isNullPtr && !other.is_null_vec()) {
            n_ = other.size();
            delete[] elements_;
            elements_ = new double[n_];

            for (int i = 0; i < n_; i++) elements_[i] = -other[i];
            isNullPtr = false;
            changed_ = true;

            return *this;
        }
        if (other.is_null_vec()) return *this;

        for (int i = 0; i < n_; i++) elements_[i] -= other[i];
        changed_ = true;

        return *this;
    }

    bool Vector::operator==(Vector &other) {
        if (!(isNullPtr && other.is_null_vec())) return false;
        if (n_ != other.size()) return false;
        if (norm() != other.norm()) return false;
        for (int i = 0; i < n_; i++) if (elements_[i] != other[i]) return false;
        return true;
    }

    bool Vector::operator!=(Vector &other) {
        return !(*this == other);
    }
} // Objects