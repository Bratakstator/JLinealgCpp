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

    struct EigenPairs {
        mutable Vector values;
        mutable Span vectors;
    };

    struct PLU {
        mutable Span P;
        mutable Span L;
        mutable Span U;
    };

    struct QR {
        mutable Span Q;
        mutable Span R;
    };

    struct SmallCache {
        mutable bool is = false;
        mutable bool valid = false;

        SmallCache& operator=(const bool b) {
            is = b;
            valid = b;
            return *this;
        }

        operator bool() const { return is && valid; } // NOLINT
    };

    struct MatrixCache {
        mutable SmallCache identity;
        mutable SmallCache diagonal;
        mutable SmallCache symmetrical;
        mutable SmallCache skew_symmetrical;

        mutable SmallCache invertible;

        mutable SmallCache upper_triangular;
        mutable SmallCache lower_triangular;

        mutable SmallCache orthonormal_columns;
        mutable SmallCache orthogonal;

        mutable det_t det = 0;
        mutable bool det_calculated = false;

        mutable bool REF_valid = false;
        mutable bool REF_with_pivots_force_eq_one = false;
        mutable Span REF;

        mutable bool RREF_valid = false;
        mutable Span RREF;

        mutable bool PLU_valid = false;
        mutable PLU plu;

        mutable bool QR_valid = false;
        mutable QR qr;

        mutable bool inverse_valid = false;
        mutable Span inverse;

        mutable bool eigen_valid = false;
        mutable EigenPairs eigen;

        void invertible_true() { // NOLINT
            identity = true;
            diagonal = true;
            symmetrical = true;

            invertible = true;

            upper_triangular = true;
            lower_triangular = true;

            orthonormal_columns = true;
            orthogonal = true;

            det = 1;
            det_calculated = true;
        }
        void invertible_false() { // NOLINT
            identity = false;
            diagonal = false;
            symmetrical = false;

            invertible = false;

            upper_triangular = false;
            lower_triangular = false;

            orthonormal_columns = false;
            orthogonal = false;

            det_calculated = false;
        }
    };

    /**
     * @brief Matrix is a fixed-sized container operating as a mathematical object.
     *
     * Does not contain default constructor, must be made either with a list of vectors or with an explicit size.
     */
    class Matrix {
        Span row_space_;
        mutable MatrixCache cache_;

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
        code_t swap_rows(size_t r1, size_t r2, code_t code=0) const;

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

        [[nodiscard]] Vector get_column(size_t n) const;
        [[nodiscard]] Vector get_row(size_t m) const;

        /**
         * @brief Gets the first instance of a non-zero row-element in given column, starting at current_row.
         */
        Code get_non_zero_row_in_col(size_t col, size_t current_row) const;
        /**
         * @brief Gets the first non-zero column-element in a given row.
         */
        Code get_first_non_zero_col(size_t row) const;

        /**
         * @brief Reduces a copy of the matrix to row echelon form.
         *
         * The reduced copy will be stored in this matrix's echelons struct in echelons_.REF and returned.\n
         * The parameter 'pivots_must_be_one' tells the method whether it should turn all pivot-elements to 1 (true),
         * or only turn elements below pivots to 0.
         */
        Matrix row_echelon(bool pivots_must_be_one=false) const;
        /**
         * @brief Reduces a copy of the matrix to reduced row echelon form.
         *
         * The reduced copy will be stored in this matrix's echelons struct in echelons_.RREF and returned.
         */
        Matrix reduced_row_echelon() const;

        /**
         * @brief Returns the transposed matrix.
         */
        Matrix transpose() const;

        /**
         * @brief Returns the determinant of the matrix.
         */
        [[nodiscard]] det_t determinant() const; // NOLINT

        /**
         * @brief Returns whether this matrix is a diagonal matrix.
         */
        [[nodiscard]] bool diagonal() const;
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