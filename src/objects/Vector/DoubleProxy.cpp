//
// Created by Joakim on 22.03.2026.
//

#include "../Matrix/Matrix.h"
#include "../Span/Span.h"
#include "Vector.h"

namespace Objects {
    ComponentProxy::operator component_t() { // NOLINT
        return component_;
    }

    ComponentProxy& ComponentProxy::operator=(const component_t component) {
        if (isNullPtr_) return *this;

        component_ = component;
        vec_changed_ = true;
        span_changed_ = true;
        return *this;
    }
    ComponentProxy& ComponentProxy::operator=(const ComponentProxy &component) {
        if (isNullPtr_) return *this;

        if (this != &component) {
            component_ = component.component_;
            vec_changed_ = true;
            span_changed_ = true;
        }
        return *this;
    }

    ComponentProxy& ComponentProxy::operator+=(const component_t component) {
        if (isNullPtr_) return *this;

        component_ += component;
        vec_changed_ = true;
        span_changed_ = true;

        return *this;
    }

    ComponentProxy& ComponentProxy::operator-=(const component_t component) {
        if (isNullPtr_) return *this;

        component_ -= component;
        vec_changed_ = true;
        span_changed_ = true;

        return *this;
    }

    ComponentProxy& ComponentProxy::operator/=(const component_t component) {
        if (component == 0) throw std::invalid_argument("Can not divide by zero");
        if (isNullPtr_) return *this;

        component_ /= component;
        vec_changed_ = true;
        span_changed_ = true;

        return *this;
    }
} // Objects