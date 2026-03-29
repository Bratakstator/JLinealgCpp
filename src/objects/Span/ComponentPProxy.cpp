//
// Created by JoakimKB on 29.03.2026.
//

#include "../Matrix/Matrix.h"
#include "Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    ComponentPProxy::operator component_t() const { return proxy_; }

    ComponentPProxy& ComponentPProxy::operator=(const component_t component) {
        proxy_ = component;
        cache_.rank.valid = false;
        return *this;
    }

    ComponentPProxy& ComponentPProxy::operator=(const ComponentProxy &component) {
        proxy_ = component;
        cache_.rank.valid = false;
        return *this;
    }

    ComponentPProxy& ComponentPProxy::operator=(const ComponentPProxy &component) {
        proxy_ = component.proxy_;
        cache_.rank.valid = false;
        return *this;
    }

    ComponentPProxy& ComponentPProxy::operator+=(const component_t component) {
        proxy_ += component;
        cache_.rank.valid = false;
        return *this;
    }

    ComponentPProxy& ComponentPProxy::operator-=(const component_t component) {
        proxy_ -= component;
        cache_.rank.valid = false;
        return *this;
    }

    ComponentPProxy& ComponentPProxy::operator/=(const component_t component) {
        proxy_ /= component;
        cache_.rank.valid = false;
        return *this;
    }
}