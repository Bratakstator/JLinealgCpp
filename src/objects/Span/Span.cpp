//
// Created by Joakim on 22.03.2026.
//

#include "../Matrix/Matrix.h"
#include "Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    Span::Span() { space_ = new Vector[1]; }

    Span::Span(const std::initializer_list<Vector> space) {
        n_ = space.size();
        space_ = new Vector[n_];

        size_t i = 0;
        for (const auto& Vec : space) {
            space_[i] = Vec;
            i++;
        }
        set_dim();
    }

    Span::Span(const Span &other) {
        n_ = other.size();
        space_ = new Vector[n_];
        rank_ = other.rank_;
        changed_ = other.changed_;

        size_t i = 0;
        for (const auto &vector : other) {
            space_[i] = vector;
            i++;
        }
    }

    Span::Span(const size_t m, const size_t n) {
        n_ = m;
        space_ = new Vector[n_];

        for (size_t i = 0; i < n_; i++) {
            space_[i] = Vector(n);
        }
    }
    Span::~Span() {
        delete[] space_;
        space_ = nullptr;
    }

    int Span::any_row_with_non_zero_in_col(const int r_, const int c_) { // NOLINT
        for (int r = r_; r < n_; r++) {
            if (space_[r][c_] != 0) return r;
        }
        return r_;
    }

    void Span::set_dim() {
        rank_ = 0;
        Span copy(*this);
    }


    Span& Span::swap(const size_t p1, const size_t p2) {
        if (n_ == 0) return *this;
        if (p1 >= n_ || p2 >= n_) {
            std::cout << "\n";
            std::stringstream err("Index out of range:\n");
            err << "Requested indexes: " << p1 << ", " << p2 << ", but the size of Span is: " << n_ << "\n";
            throw std::out_of_range(err.str());
        }

        const auto tmp = space_[p1];
        space_[p1] = space_[p2];
        space_[p2] = tmp;

        return *this;
    }


    [[nodiscard]] dim_t Span::rank() const { return rank_; }
    [[nodiscard]] dim_t Span::vector_dimension() const { return space_[0].dimension(); }
    [[nodiscard]] size_t Span::size() const { return n_; }


    [[nodiscard]] Vector* Span::begin() const { return space_; }
    [[nodiscard]] Vector* Span::end() const { return space_ + n_; }
    [[nodiscard]] Vector* Span::begin() { return space_; }
    [[nodiscard]] Vector* Span::end() { return space_ + n_; }
} // Objects