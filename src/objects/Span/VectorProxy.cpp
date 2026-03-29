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
        cache_.rank.valid = false;
        return *this;
    }
    VectorProxy& VectorProxy::operator=(const VectorProxy &other) {
        if (this != &other) {
            vector_ = other.vector_;
            cache_.rank.valid = false;
        }
        return *this;
    }

    ComponentPProxy VectorProxy::operator[](size_t i) { // NOLINT
        ComponentProxy proxy(vector_[i]);
        return {proxy, cache_};
    }
    double VectorProxy::operator[](const size_t i) const {
        return vector_[i];
    }
} // Objects