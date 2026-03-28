//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_MATRIX_H
#define JLINEALGCPP_MATRIX_H

#include "../Span/Span.h"

namespace Objects {
    using det_t = component_t;
    using code_t = int;

    /**
     * Struct containing an index / value and return code of a method.\n
     * This is with the assumption of the method using this struct is capable of self-correcting if code indicates
     * failure.
     */
    struct Code {
        size_t index;
        code_t code;
    };

    /**
     * @brief Struct containing the echelon forms of a matrix.
     *
     * The struct also comes with these flags:
     * - REF_valid
     * - RREF_valid
     * - REF_with_pivots_eq_one
     *
     * These flags must be changed if the matrix itself is changed (except for row operations), this as they are used
     * to indicate the validity of the echelon forms, which they will not be unless the transformation of the matrix
     * remains row equivalent.
     *
     * @param REF is the row echelon form, an upper-triangular matrix.
     * @param RREF is the reduced row echelon form where each pivot element will be the only non-zero element in column.\n
     * @param REF_valid should indicate whether the REF is valid or not.
     * @param RREF_valid should indicate whether the RREF is valid or not.
     * @param REF_with_pivots_eq_one should indicate if REF was formed by turning each pivot to one, this so
     * the determinant-method won't use it to calculate the determinant.
     */
    struct Echelons {
        Span REF;
        Span RREF;

        void reset(const bool REF_B=true, const bool RREF_B=true) {
            if (REF_B) {
                REF_valid = false;
                REF_with_pivots_eq_one = false;
            }
            if (RREF_B) RREF_valid = false;
        }

        bool REF_valid = false;
        bool REF_with_pivots_eq_one = false;
        bool RREF_valid = false;
    };

    /**
     * @brief Matrix is a fixed-sized container operating as a mathematical object.
     *
     * Does not contain default constructor, must be made either with a list of vectors or with an explicit size.
     */
    class Matrix {
        Span row_space_;
        Echelons echelons_;

        int identity_ = -1; // -1: not calculated, 0: false, 1: true
        int diagonal_ = -1;

        det_t determinant_ = 0;
        bool determinant_calculated = false;

        /**
         * @brief Swaps rows in the matrix.
         *
         * returns code, the codes are:\n
         * 0: successful swap.\n
         * 1: whether this indicates failure is context-specific. This code indicates that the indexes for r1 and r2
         * are the same, i.e. they point to the same row.\n
         * O: if the code passed as an argument is a non-zero value, the method returns 128 + code, this so it can't
         * be confused with its own codes, the actual code can then be retrieved with:\n
         * <code> code_t code = swap_rows(2, 4, 15); // returns 128 + 15 = 143 </code>\n
         * <code> code_t code_actual = code - 128; // code_actual = 143 - 128 = 15 </code>
         */
        code_t swap_rows(size_t r1, size_t r2, code_t code=0);

    public:
        Matrix() = delete;

        /**
         * Creates an nxn identity matrix.
         */
        explicit Matrix(size_t n);
        /**
         * Creates an all-zero mxn matrix.
         */
        Matrix(size_t m, size_t n);
        /**
         * Creates a new matrix from the span of another row space.
         */
        explicit Matrix(const Span &row_space);
        /**
         * Creates a matrix from a list of vectors, using initializer list.
         */
        explicit Matrix(std::initializer_list<Vector> row_space);
        /**
         * Creates a copy of another matrix.
         */
        Matrix(const Matrix &other);

        ComponentProxy operator[](size_t m, size_t n); // NOLINT
        component_t operator[](size_t m, size_t n) const;
        Matrix operator*(const Matrix &other);

        Vector get_column(size_t n) const;
        Vector get_row(size_t m) const;

        /**
         * @brief Gets the first instance of a non-zero row-element in given column, starting at current_row.
         */
        Code get_non_zero_row_in_col(size_t col, size_t current_row);
        /**
         * @brief Gets the first non-zero column-element in a given row.
         */
        Code get_first_non_zero_col(size_t row);

        /**
         * @brief Reduces a copy of the matrix to row echelon form.
         *
         * The reduced copy will be stored in this matrix's echelons struct in echelons_.REF and returned.\n
         * The parameter 'pivots_must_be_one' tells the method whether it should turn all pivot-elements to 1 (true),
         * or only turn elements below pivots to 0.
         */
        Matrix row_echelon(bool pivots_must_be_one=false);
        /**
         * @brief Reduces a copy of the matrix to reduced row echelon form.
         *
         * The reduced copy will be stored in this matrix's echelons struct in echelons_.RREF and returned.
         */
        Matrix reduced_row_echelon();

        /**
         * @brief Returns the transposed matrix.
         */
        Matrix transpose();

        /**
         * @brief Returns the determinant of the matrix.
         */
        [[nodiscard]] det_t determinant(); // NOLINT

        /**
         * @brief Returns whether this matrix is a diagonal matrix.
         */
        [[nodiscard]] bool diagonal();
        /**
         * Returns whether this matrix is an identity matrix.
         */
        [[nodiscard]] bool identity();
        /**
         * Returns whether this matrix is an identity matrix.
         */
        [[nodiscard]] bool identity() const;

        [[nodiscard]] size_t rows() const;
        [[nodiscard]] size_t columns() const;
        [[nodiscard]] size_t rank() const;

        void print() const;
    };
} // Objects

#endif //JLINEALGCPP_MATRIX_H