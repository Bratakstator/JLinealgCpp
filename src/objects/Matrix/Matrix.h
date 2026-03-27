//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_MATRIX_H
#define JLINEALGCPP_MATRIX_H

#include "../Span/Span.h"

namespace Objects {
    using det_t = component_t;
    using code_t = int;

    struct Code {
        size_t index;
        code_t code;
    };

    struct Echelons {
        Span REF;
        Span RREF;

        bool REF_calculated = false;
        bool REF_with_pivots_eq_one = false;
        bool RREF_calculated = false;
    };

    class Matrix {
        Span row_space_;
        Echelons echelons_;

        int identity_ = -1; // -1: not calculated, 0: false, 1: true
        int diagonal_ = -1;

        det_t determinant_ = 0;
        bool determinant_calculated = false;

        code_t swap_rows(size_t r1, size_t r2, code_t code=0);

    public:
        Matrix() = delete;

        explicit Matrix(size_t n);
        Matrix(size_t m, size_t n);
        explicit Matrix(const Span &row_space);
        explicit Matrix(std::initializer_list<Vector> row_space);
        Matrix(const Matrix &other);

        ComponentProxy operator[](size_t m, size_t n); // NOLINT
        component_t operator[](size_t m, size_t n) const;
        Matrix operator*(const Matrix &other);

        Code get_non_zero_row_in_col(size_t col, size_t current_row);
        Code get_first_non_zero_col(size_t row);

        Matrix row_echelon(bool pivots_must_be_one=false);
        Matrix reduced_row_echelon();

        [[nodiscard]] det_t determinant(); // NOLINT

        [[nodiscard]] bool diagonal();
        [[nodiscard]] bool identity();
        [[nodiscard]] bool identity() const;

        [[nodiscard]] size_t rows() const;
        [[nodiscard]] size_t columns() const;

        void print() const;
    };
} // Objects

#endif //JLINEALGCPP_MATRIX_H