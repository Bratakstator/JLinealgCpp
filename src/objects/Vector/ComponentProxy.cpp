//
// Created by Joakim on 22.03.2026.
//

#include "../Matrix/Matrix.h"
#include "../Span/Span.h"
#include "Vector.h"

namespace Objects {
    ComponentProxy::operator component_t() const {
        return component_;
    }

    ComponentProxy& ComponentProxy::operator=(const component_t component) {
        if (cache_.norm.valid && cache_.norm.is == 0) return *this;

        component_ = component;
        cache_.norm.valid = false;
        return *this;
    }
    ComponentProxy& ComponentProxy::operator=(const ComponentProxy &component) {
        if (cache_.norm.valid && cache_.norm.is == 0) return *this;

        if (this != &component) {
            component_ = component.component_;
            cache_.norm.valid = false;
        }
        return *this;
    }

    ComponentProxy& ComponentProxy::operator+=(const component_t component) {
        if (cache_.norm.valid && cache_.norm.is == 0) return *this;

        component_ += component;
        cache_.norm.valid = false;
        return *this;
    }

    ComponentProxy& ComponentProxy::operator-=(const component_t component) {
        if (cache_.norm.valid && cache_.norm.is == 0) return *this;

        component_ -= component;
        cache_.norm.valid = false;
        return *this;
    }

    ComponentProxy& ComponentProxy::operator/=(const component_t component) {
        if (component == 0) throw std::invalid_argument("Can not divide by zero");
        if (cache_.norm.valid && cache_.norm.is == 0) return *this;

        component_ /= component;
        cache_.norm.valid = false;
        return *this;
    }
} // Objects