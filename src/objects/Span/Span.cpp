//
// Created by Joakim on 22.03.2026.
//

#include <iostream>

#include "../Matrix/Matrix.h"

namespace Objects {
    Span::Span() {
        space_ = new Vector[1];

        cache_ = {
            {0, true},
            {0, true}
        };
    }

    Span::Span(const std::initializer_list<Vector> space) {
        cache_ = {
            {space.size(), true},
            {0, false}
        };
        space_ = new Vector[cache_.count.is];

        size_t i = 0;
        for (const auto& Vec : space) {
            space_[i] = Vec;
            i++;
        }
        set_dim();
    }

    Span::Span(const Span &other) {
        cache_ = other.cache_;
        space_ = new Vector[cache_.count.is];

        size_t i = 0;
        for (const auto &vector : other) {
            space_[i] = vector;
            i++;
        }
    }

    Span::Span(const size_t m, const size_t n) {
        cache_ = {
            {m, true},
            {0, false}
        };
        space_ = new Vector[cache_.count.is];

        for (size_t i = 0; i < cache_.count.is; i++) {
            space_[i] = Vector(n);
        }
    }

    Span::~Span() {
        delete[] space_;
        space_ = nullptr;
    }

    int Span::any_row_with_non_zero_in_col(const int r_, const int c_) const { // NOLINT
        for (int r = r_; r < cache_.count.is; r++) {
            if (space_[r][c_] != 0) return r;
        }
        return r_;
    }

    void Span::set_dim() const {
        cache_.rank = {0, true};
        Span copy(*this);
    }

    Span& Span::swap(const size_t p1, const size_t p2) {
        if (cache_.count.is == 0) return *this;
        if (p1 >= cache_.count.is || p2 >= cache_.count.is) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested indexes: " << p1 << ", " << p2 << ", but the size of Span is: " << cache_.count.is << "\n";
            throw std::out_of_range(err.str());
        }

        const auto tmp = space_[p1];
        space_[p1] = space_[p2];
        space_[p2] = tmp;

        return *this;
    }

    [[nodiscard]] dim_t Span::rank() const {
        cache_.rank.valid = false;
        if (!cache_.rank.valid) {
            Matrix A = Matrix(*this).row_echelon();
            cache_.rank.is = 0;
            for (size_t row = 0; row < A.rows(); row++) {
                if (A.get_row(row).norm() != 0) cache_.rank.is++;
            }
        }

        cache_.rank.valid = true;
        return cache_.rank.is;
    }
    [[nodiscard]] dim_t Span::vector_dimension() const { return space_[0].dimension(); }
    [[nodiscard]] size_t Span::size() const { return cache_.count.is; }

    [[nodiscard]] Vector* Span::begin() const { return space_; }
    [[nodiscard]] Vector* Span::end() const { return space_ + cache_.count.is; }
    [[nodiscard]] Vector* Span::begin() { return space_; }
    [[nodiscard]] Vector* Span::end() { return space_ + cache_.count.is; }
} // Objects