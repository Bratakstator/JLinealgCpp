//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_MATRIX_H
#define JLINEALGCPP_MATRIX_H

#include "../Span/Span.h"

namespace Objects {
    class Matrix {
        Span span_;

        int identity_ = 1; // -1: not calculated, 0: false, 1: true

    public:
        Matrix();
        explicit Matrix(const Span &span);
        explicit Matrix(std::initializer_list<Vector> span);
        Matrix(const Matrix &other);

        DoubleProxy operator[](int m, int n); // NOLINT
        double operator[](int m, int n) const;
        Matrix operator*(const Matrix &other);

        [[nodiscard]] bool identity();
        [[nodiscard]] bool identity() const;

        [[nodiscard]] int rows() const;
        [[nodiscard]] int columns() const;
    };
} // Objects

#endif //JLINEALGCPP_MATRIX_H