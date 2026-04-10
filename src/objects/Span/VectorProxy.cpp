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

    VectorProxy& VectorProxy::operator-=(const VectorProxy &other) {
        cache_.rank.valid = false;
        vector_ -= other.vector_;
        return *this;
    }

    VectorProxy& VectorProxy::operator-=(const Vector &other) {
        cache_.rank.valid = false;
        vector_ -= other;
        return *this;
    }

    VectorProxy& VectorProxy::operator/=(const component_t component) {
        vector_ /= component;
        cache_.rank.valid = false;
        return *this;
    }

    VectorProxy& VectorProxy::operator*=(const component_t component) {
        cache_.rank.valid = false;
        vector_ *= component;
        return *this;
    }

    ComponentPProxy VectorProxy::operator[](size_t i) { // NOLINT
        return {vector_[i].component_, cache_, vector_[i].cache_};
    }
    double VectorProxy::operator[](const size_t i) const {
        return vector_[i];
    }
} // Objects