//
// Created by Joakim on 22.03.2026.
//

#include "Span.h"

namespace Objects {
    Span& Span::operator=(const Span &other) {
        if (this == &other) return *this;

        const auto old_n_ = n_;
        n_ = other.n_;
        if (old_n_ != n_) {
            delete[] space_;
            space_ = new Vector[n_];
        }

        dim_ = other.dim_;
        dim_changed = other.dim_changed;

        for (int i = 0; i < n_; i++) space_[i] = other[i];
        return *this;
    }


    VectorProxy Span::operator[](int p) { // NOLINT
        if (n_ == 0) return {*space_, dim_changed};
        if (p >= n_) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << p << ", but size of Span is: " << n_ << "\n";
            throw std::out_of_range(err.str());
        }
        return {space_[p], dim_changed};
    }
    Vector Span::operator[](const int p) const {
        if (n_ == 0) return *space_;
        if (p >= n_) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << p << ", but the size of Span is: " << n_ << "\n";
            throw std::out_of_range(err.str());
        }
        return space_[p];
    }
} // Objects