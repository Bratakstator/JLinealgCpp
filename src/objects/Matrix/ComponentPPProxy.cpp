//
// Created by JoakimKB on 29.03.2026.
//

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    ComponentPPProxy::operator component_t() const { return proxy_; } // NOLINT

    ComponentPPProxy& ComponentPPProxy::operator=(component_t component) {
        proxy_ = component;
        cache_.invalidate();
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator=(const ComponentProxy &component) {
        proxy_ = component;
        cache_.invalidate();
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator=(const ComponentPProxy &component) {
        proxy_ = component;
        cache_.invalidate();
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator=(const ComponentPPProxy &component) {
        proxy_ = component.proxy_;
        cache_.invalidate();
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator+=(component_t component) {
        proxy_ += component;
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator-=(component_t component) {
        proxy_-= component;
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator/=(component_t component) {
        proxy_ /= component;
        return *this;
    }
}