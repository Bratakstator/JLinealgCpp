//
// Created by Joakim on 22.03.2026.
//

#include "../Matrix/Matrix.h"
#include "../Span/Span.h"
#include "Vector.h"

namespace Objects {
    DoubleProxy::operator double() { // NOLINT
        return element_;
    }

    DoubleProxy& DoubleProxy::operator=(const double val) {
        if (isNullPtr_) return *this;

        element_ = val;
        changed_ = true;
        dim_changed_ = true;
        return *this;
    }
    DoubleProxy& DoubleProxy::operator=(const DoubleProxy &other) {
        if (isNullPtr_) return *this;

        if (this != &other) {
            element_ = other.element_;
            changed_ = true;
            dim_changed_ = true;
        }
        return *this;
    }

    DoubleProxy& DoubleProxy::operator+=(const double val) {
        if (isNullPtr_) return *this;

        element_ += val;
        changed_ = true;
        dim_changed_ = true;

        return *this;
    }

    DoubleProxy& DoubleProxy::operator-=(double val) {
        if (isNullPtr_) return *this;

        element_ -= val;
        changed_ = true;
        dim_changed_ = true;

        return *this;
    }

    DoubleProxy& DoubleProxy::operator/=(const double val) {
        if (val == 0) throw std::invalid_argument("Can not divide by zero");
        if (isNullPtr_) return *this;

        element_ /= val;
        changed_ = true;
        dim_changed_ = true;

        return *this;
    }
} // Objects