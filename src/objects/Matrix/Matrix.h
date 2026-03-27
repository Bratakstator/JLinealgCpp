//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_MATRIX_H
#define JLINEALGCPP_MATRIX_H

#include "../Span/Span.h"

namespace Objects {
    struct Echelons {
        Span REF;
        Span RREF;

        bool REF_calculated = false;
        bool REF_with_pivots_eq_one = false;
        bool RREF_calculated = false;
    };

    class Matrix {
        Span span_;
        Echelons echelons_;

        int identity_ = -1; // -1: not calculated, 0: false, 1: true
        int diagonal_ = -1;

        double determinant_ = 0;
        bool determinant_calculated = false;

        int swap_rows(int r1, int r2);

    public:
        Matrix();
        explicit Matrix(const Span &span);
        explicit Matrix(std::initializer_list<Vector> span);
        Matrix(const Matrix &other);

        DoubleProxy operator[](int m, int n); // NOLINT
        double operator[](int m, int n) const;
        Matrix operator*(const Matrix &other);

        int get_non_zero_row_in_col(int col, int current_row);
        int get_first_non_zero_col(int row);

        Matrix row_echelon(bool pivots_must_be_one=false);
        Matrix reduced_row_echelon();

        [[nodiscard]] double determinant(); // NOLINT

        [[nodiscard]] bool diagonal();
        [[nodiscard]] bool identity();
        [[nodiscard]] bool identity() const;

        [[nodiscard]] int rows() const;
        [[nodiscard]] int columns() const;

        void print() const;
    };
} // Objects

#endif //JLINEALGCPP_MATRIX_H