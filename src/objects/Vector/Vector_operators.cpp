//
// Created by Joakim on 22.03.2026.
//

#include "../Matrix/Matrix.h"
#include "../Span/Span.h"
#include "Vector.h"

namespace Objects {
    Vector& Vector::operator=(const Vector &other) {
        if (this == &other) return *this;
        cache_ = other.cache_;

        components_ = new component_t[cache_.n.is];

        for (size_t i = 0; i < cache_.n.is; i++) {
            components_[i] = other.components_[i];
        }
        return *this;
    }


    ComponentProxy Vector::operator[](size_t i) { // NOLINT
        if (cache_.n.is == 0) return {*components_, cache_};
        if (i >= cache_.n.is) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << i << ", but size of Vector is: " << cache_.n.is << "\n";
            throw std::out_of_range(err.str());
        }
        return {components_[i], cache_};
    }
    double Vector::operator[](const size_t i) const {
        if (cache_.n.is == 0) return 0;
        if (i >= cache_.n.is) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << i << ", but size of Vector is: " << cache_.n.is << "\n";
            throw std::out_of_range(err.str());
        }
        return components_[i];
    }


    Vector& Vector::operator+=(const Vector &other) {
        if (is_null_vec() && !other.is_null_vec()) {
            cache_.n = other.cache_.n;
            delete[] components_;
            components_ = new component_t[cache_.n.is];

            for (size_t i = 0; i < cache_.n.is; i++) components_[i] = other[i];
            cache_.norm.valid = false;

            return *this;
        }
        if (other.is_null_vec()) return *this;

        for (size_t i = 0; i < cache_.n.is; i++) components_[i] += other[i];
        cache_.norm.valid = false;

        return *this;
    }

    Vector& Vector::operator-=(const Vector &other) {
        if (is_null_vec() && !other.is_null_vec()) {
            cache_.n = other.cache_.n;
            delete[] components_;
            components_ = new component_t[cache_.n.is];

            for (size_t i = 0; i < cache_.n.is; i++) components_[i] = -other[i];
            cache_.norm.valid = false;

            return *this;
        }
        if (other.is_null_vec()) return *this;

        for (size_t i = 0; i < cache_.n.is; i++) components_[i] -= other[i];
        cache_.norm.valid = false;

        return *this;
    }

    Vector& Vector::operator*=(const component_t c) {
        if (!is_null_vec()) for (size_t i = 0; i < cache_.n.is; i++) components_[i] *= c;
        return *this;
    }

    Vector& Vector::operator/=(const component_t c) {
        if (!is_null_vec()) for (size_t i = 0; i < cache_.n.is; i++) components_[i] /= c;
        return *this;
    }


    bool Vector::operator==(Vector &other) const {
        if (!(is_null_vec() && other.is_null_vec())) return false;
        if (cache_.n.is != other.dimension()) return false;
        if (norm() != other.norm()) return false;
        for (size_t i = 0; i < cache_.n.is; i++) if (components_[i] != other[i]) return false;
        return true;
    }

    bool Vector::operator!=(Vector &other) const {
        return !(*this == other);
    }
} // Objects