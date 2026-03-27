//
// Created by Joakim on 22.03.2026.
//

#include "../Matrix/Matrix.h"
#include "../Span/Span.h"
#include "Vector.h"

namespace Objects {
    Vector& Vector::operator=(const Vector &other) {
        if (this == &other) return *this;
        n_ = other.n_;
        norm_ = other.norm_;
        changed_ = other.changed_;
        isNullPtr = other.isNullPtr;

        components_ = new component_t[n_];

        for (size_t i = 0; i < n_; i++) {
            components_[i] = other.components_[i];
        }
        return *this;
    }


    ComponentProxy Vector::operator[](size_t i) { // NOLINT
        if (n_ == 0) return {*components_, changed_, isNullPtr};
        if (i >= n_) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << i << ", but size of Vector is: " << n_ << "\n";
            throw std::out_of_range(err.str());
        }
        return {components_[i], changed_, isNullPtr};
    }
    double Vector::operator[](const size_t i) const {
        if (n_ == 0) return 0;
        if (i >= n_) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << i << ", but size of Vector is: " << n_ << "\n";
            throw std::out_of_range(err.str());
        }
        return components_[i];
    }

    Vector& Vector::operator+=(const Vector &other) {
        if (isNullPtr && !other.isNullPtr) {
            n_ = other.n_;
            delete[] components_;
            components_ = new component_t[n_];

            for (size_t i = 0; i < n_; i++) components_[i] = other[i];
            isNullPtr = false;
            changed_ = true;

            return *this;
        }
        if (other.isNullPtr) return *this;

        for (size_t i = 0; i < n_; i++) components_[i] += other[i];
        changed_ = true;

        return *this;
    }

    Vector& Vector::operator-=(const Vector &other) {
        if (isNullPtr && !other.isNullPtr) {
            n_ = other.n_;
            delete[] components_;
            components_ = new component_t[n_];

            for (size_t i = 0; i < n_; i++) components_[i] = -other[i];
            isNullPtr = false;
            changed_ = true;

            return *this;
        }
        if (other.isNullPtr) return *this;

        for (size_t i = 0; i < n_; i++) components_[i] -= other[i];
        changed_ = true;

        return *this;
    }

    bool Vector::operator==(Vector &other) {
        if (!(isNullPtr && other.is_null_vec())) return false;
        if (n_ != other.dimension()) return false;
        if (norm() != other.norm()) return false;
        for (size_t i = 0; i < n_; i++) if (components_[i] != other[i]) return false;
        return true;
    }

    bool Vector::operator!=(Vector &other) {
        return !(*this == other);
    }
} // Objects