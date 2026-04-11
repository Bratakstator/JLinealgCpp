//
// Created by JoakimKB on 29.03.2026.
//

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    ComponentPPProxy::operator component_t() const { return proxy_; } // NOLINT

    ComponentPPProxy& ComponentPPProxy::operator=(const component_t component) {
        proxy_ = component;
        mCache_.invalidate();
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator=(const ComponentPPProxy &component) {
        proxy_ = component.proxy_;
        mCache_.invalidate();
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator+=(const component_t component) {
        proxy_ += component;
        mCache_.invalidate();
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator-=(component_t component) {
        proxy_-= component;
        mCache_.invalidate();
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator*=(const component_t component) {
        proxy_ *= component;
        mCache_.invalidate();
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }

    ComponentPPProxy& ComponentPPProxy::operator/=(component_t component) {
        proxy_ /= component;
        mCache_.invalidate();
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }
}