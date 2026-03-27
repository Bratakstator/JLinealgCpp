//
// Created by Joakim on 22.03.2026.
//

#include "../Matrix/Matrix.h"
#include "Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    Span& Span::operator=(const Span &other) {
        if (this == &other) return *this;

        const auto old_n_ = n_;
        n_ = other.n_;
        if (old_n_ != n_) {
            delete[] space_;
            space_ = new Vector[n_];
        }

        rank_ = other.rank_;
        changed_ = other.changed_;

        for (size_t i = 0; i < n_; i++) space_[i] = other[i];
        return *this;
    }


    VectorProxy Span::operator[](size_t i) { // NOLINT
        if (n_ == 0) return {*space_, changed_};
        if (i >= n_) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << i << ", but size of Span is: " << n_ << "\n";
            throw std::out_of_range(err.str());
        }
        return {space_[i], changed_};
    }
    Vector Span::operator[](const size_t i) const {
        if (n_ == 0) return *space_;
        if (i >= n_) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested index: " << i << ", but the size of Span is: " << n_ << "\n";
            throw std::out_of_range(err.str());
        }
        return space_[i];
    }
} // Objects