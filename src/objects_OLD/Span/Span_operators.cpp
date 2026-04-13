//
// Created by Joakim on 22.03.2026.
//

#include <iostream>

#include "Span.h"

namespace Objects_OLD {
    Span& Span::operator=(const Span &other) {
        if (this == &other) return *this;

        const auto old_n_ = cache_.count.is;
        cache_.count = other.cache_.count;
        if (old_n_ != cache_.count.is) {
            delete[] space_;
            space_ = new Vector[cache_.count.is];
        }

        cache_.rank = other.cache_.rank;

        for (size_t i = 0; i < cache_.count.is; i++) space_[i] = other[i];
        return *this;
    }

    VectorProxy Span::operator[](size_t i) { // NOLINT
        if (cache_.count.is == 0) return {*space_, cache_};
        if (i >= cache_.count.is) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << i << ", but size of Span is: " << cache_.count.is << "\n";
            throw std::out_of_range(err.str());
        }
        return {space_[i], cache_};
    }

    Vector Span::operator[](const size_t i) const {
        if (cache_.count.is == 0) return *space_;
        if (i >= cache_.count.is) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << i << ", but the size of Span is: " << cache_.count.is << "\n";
            throw std::out_of_range(err.str());
        }
        return space_[i];
    }
} // Objects