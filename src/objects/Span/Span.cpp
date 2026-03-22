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

        int i = 0;
        for (const auto& Vec : space) {
            space_[i] = Vec;
            i++;
        }
        set_dim();
    }

    Span::Span(const Span &other) {
        n_ = other.size();
        space_ = new Vector[n_];
        dim_ = other.dim_;
        dim_changed = other.dim_changed;

        int i = 0;
        for (auto &vector : other) {
            space_[i] = vector;
            i++;
        }
    }

    Span::Span(const int m, const int n) {
        n_ = m;
        space_ = new Vector[n_];

        for (int i = 0; i < n_; i++) {
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
        dim_ = 0;
        Span copy(*this);
    }


    Span& Span::swap(const int p1, const int p2) {
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


    [[nodiscard]] int Span::dim() const { return dim_; }
    [[nodiscard]] int Span::room() const { return space_[0].size(); }
    [[nodiscard]] int Span::size() const { return n_; }


    [[nodiscard]] Vector* Span::begin() const { return space_; }
    [[nodiscard]] Vector* Span::end() const { return space_ + n_; }
    [[nodiscard]] Vector* Span::begin() { return space_; }
    [[nodiscard]] Vector* Span::end() { return space_ + n_; }
} // Objects