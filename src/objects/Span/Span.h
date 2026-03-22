//
// Created by Joakim on 3/10/2026.
//

#ifndef JLINEALGCPP_SPAN_H
#define JLINEALGCPP_SPAN_H

#include <iostream>

#include "../Vector/Vector.h"

//  ________________
// {| a1 | a2 | a3 |
//  | a4 | a5 | a6 |
//  | a7 | a8 | a9 |}
//  ----------------

namespace Objects {
    class VectorProxy {
        Vector &vector_;
        bool &dim_changed_;

    public:
        VectorProxy(Vector &vector, bool &dim_changed)
            : vector_(vector), dim_changed_(dim_changed)
        {}

        operator Vector() { return vector_; } // NOLINT

        VectorProxy& operator=(const Vector &other) {
            vector_ = other;
            dim_changed_ = true;
            return *this;
        }
        VectorProxy& operator=(const VectorProxy &other) {
            if (this != &other) {
                vector_ = other.vector_;
                dim_changed_ = true;
            }
            return *this;
        }

        DoubleProxy operator[](int p) { // NOLINT
            const auto proxy(vector_[p]);
            return {proxy.element_, proxy.changed_, dim_changed_, proxy.isNullPtr_};
        }
        double operator[](const int p) const {
            return vector_[p];
        }
    };

    class Span {
        Vector *space_ = nullptr;
        int n_ = 0;
        int dim_ = 0;

        bool dim_changed = true;

        int any_row_with_non_zero_in_col(const int r_, const int c_) { // NOLINT
            for (int r = r_; r < n_; r++) {
                if (space_[r][c_] != 0) return r;
            }
            return r_;
        }

        void set_dim() {
            dim_ = 0;
            Span copy(*this);
        }

    public:
        Span() { space_ = new Vector[1]; }
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
        explicit Span(const Span &other) {
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
        explicit Span(const int m, const int n) {
            n_ = m;
            space_ = new Vector[n_];

            for (int i = 0; i < n_; i++) {
                space_[i] = Vector(n);
            }
        }
        ~Span() {
            delete[] space_;
            space_ = nullptr;
        }

        Span& operator=(const Span &other) {
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


        VectorProxy operator[](int p) { // NOLINT
            if (n_ == 0) return {*space_, dim_changed};
            if (p >= n_) {
                std::cout << "\n";
                std::stringstream err("Index out of range:\n");
                err << "Requested index: " << p << ", but size of Span is: " << n_ << "\n";
                throw std::out_of_range(err.str());
            }
            return {space_[p], dim_changed};
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


        Span& swap(const int p1, const int p2) {
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


        [[nodiscard]] Vector* begin() const { return space_; }
        [[nodiscard]] Vector* end() const { return space_ + n_; }
        [[nodiscard]] Vector* begin() { return space_; }
        [[nodiscard]] Vector* end() { return space_ + n_; }
    };
} // Objects

#endif //JLINEALGCPP_SPAN_H