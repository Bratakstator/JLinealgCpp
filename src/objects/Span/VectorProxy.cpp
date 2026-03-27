//
// Created by Joakim on 22.03.2026.
//

#include "../Matrix/Matrix.h"
#include "Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    VectorProxy::operator Vector() { return vector_; } // NOLINT

    VectorProxy& VectorProxy::operator=(const Vector &other) {
        vector_ = other;
        changed_ = true;
        return *this;
    }
    VectorProxy& VectorProxy::operator=(const VectorProxy &other) {
        if (this != &other) {
            vector_ = other.vector_;
            changed_ = true;
        }
        return *this;
    }

    ComponentProxy VectorProxy::operator[](size_t i) { // NOLINT
        const auto proxy(vector_[i]);
        return {proxy.component_, proxy.vec_changed_, changed_, proxy.isNullPtr_};
    }
    double VectorProxy::operator[](const size_t i) const {
        return vector_[i];
    }
} // Objects