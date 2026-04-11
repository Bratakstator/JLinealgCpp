//
// Created by JoakimKB on 29.03.2026.
//

#include "Span.h"

namespace Objects {
    ComponentPProxy::operator component_t() const { return proxy_; }

    ComponentPProxy& ComponentPProxy::operator=(const component_t component) {
        proxy_ = component;
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }

    ComponentPProxy& ComponentPProxy::operator=(const ComponentPProxy &component) {
        proxy_ = component.proxy_;
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }

    ComponentPProxy& ComponentPProxy::operator+=(const component_t component) {
        proxy_ += component;
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }

    ComponentPProxy& ComponentPProxy::operator-=(const component_t component) {
        proxy_ -= component;
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }

    ComponentPProxy& ComponentPProxy::operator/=(const component_t component) {
        proxy_ /= component;
        sCache_.rank.valid = false;
        vCache_.norm.valid = false;
        return *this;
    }
}