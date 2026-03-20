//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_MATRIX_H
#define JLINEALGCPP_MATRIX_H

#include "Span.h"

namespace Objects {
    class Matrix {
        Span space_;

    public:
        Matrix() = default;
        explicit Matrix(const Span &space) {
            space_ = space;
        }
        explicit Matrix(const std::initializer_list<Vector> space) {
            space_ = Span(space);
        }


        double operator[](int m, int n) { // NOLINT
            return space_[m][n];
        }
        double operator[](const int m, const int n) const {
            return space_[m][n];
        }

        Matrix operator*(const Matrix &other) { // NOLINT
            Span space;

            return Matrix(space);
        }
    };
} // Objects

#endif //JLINEALGCPP_MATRIX_H