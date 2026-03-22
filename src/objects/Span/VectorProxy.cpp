//
// Created by Joakim on 22.03.2026.
//

#include "Span.h"

namespace Objects {
    VectorProxy::operator Vector() { return vector_; } // NOLINT

    VectorProxy& VectorProxy::operator=(const Vector &other) {
        vector_ = other;
        dim_changed_ = true;
        return *this;
    }
    VectorProxy& VectorProxy::operator=(const VectorProxy &other) {
        if (this != &other) {
            vector_ = other.vector_;
            dim_changed_ = true;
        }
        return *this;
    }

    DoubleProxy VectorProxy::operator[](int p) { // NOLINT
        const auto proxy(vector_[p]);
        return {proxy.element_, proxy.changed_, dim_changed_, proxy.isNullPtr_};
    }
    double VectorProxy::operator[](const int p) const {
        return vector_[p];
    }
} // Objects