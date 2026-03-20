//
// Created by Joakim on 3/10/2026.
//

#ifndef JLINEALGCPP_SPAN_H
#define JLINEALGCPP_SPAN_H

#include <iostream>

#include "Vector.h"

namespace Objects {
    class Span {
        Vector *space_ = nullptr;
        int n_ = 0;
        int dim_ = 0;

        bool dim_calculated = false;

        int any_row_with_non_zero_in_col(const int r_, const int c_) { // NOLINT
            for (int r = r_; r < n_; r++) {
                if (space_[r][c_] != 0) return r;
            }
            return r_;
        }

        void set_dim() {
            dim_ = 0;
            for (int rPivot = 0; rPivot < n_; rPivot++) {
                if (space_[rPivot].norm() != 0) dim_++;
                for (int cPivot = rPivot; cPivot < room(); cPivot++) {
                    auto element = space_[rPivot][cPivot];
                    if (element == 0) {
                        auto r_swap = any_row_with_non_zero_in_col(rPivot, cPivot);
                        if (rPivot != r_swap) swap(rPivot, r_swap);
                        else continue;
                    }

                    for (int c = cPivot; c < room(); c++) {
                        space_[rPivot][c] = space_[rPivot][c] / element;
                    }
                    for (int r = rPivot + 1; r < n_; r++) {
                        auto div = space_[r][cPivot];
                        if (div != 0) {
                            for (int c = cPivot; c < room(); c++) {
                                space_[r][c] = (space_[r][c] / div) - space_[r - rPivot][c];
                            }
                        }
                    }
                }
            }
        }

    public:
        Span() { space_ = new Vector(); }
        explicit Span(const std::initializer_list<Vector> space) {
            n_ = space.size();
            space_ = new Vector[n_];

            int i = 0;
            for (const auto& Vec : space) {
                space_[i] = Vec;
                i++;
            }
            set_dim();
        }
        ~Span() { delete space_; }


        Vector operator[](int p) { // NOLINT
            if (n_ == 0) return *space_;
            if (p >= n_) {
                std::cout << "\n";
                std::stringstream err("Index out of range:\n");
                err << "Requested index: " << p << ", but size of Span is: " << n_ << "\n";
                throw std::out_of_range(err.str());
            }
            return space_[p];
        }
        Vector operator[](const int p) const {
            if (n_ == 0) return *space_;
            if (p >= n_) {
                std::cout << "\n";
                std::stringstream err("Index out of range:\n");
                err << "Requested index: " << p << ", but the size of Span is: " << n_ << "\n";
                throw std::out_of_range(err.str());
            }
            return space_[p];
        }


        Span swap(const int p1, const int p2) {
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


        [[nodiscard]] int dim() const { return dim_; }
        [[nodiscard]] int room() const { return space_[0].size(); }
        [[nodiscard]] int size() const { return n_; }
    };
} // Objects

#endif //JLINEALGCPP_SPAN_H