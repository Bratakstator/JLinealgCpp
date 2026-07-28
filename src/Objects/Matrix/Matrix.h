//
// Created by JoakimKB on 13.04.2026.
//

#ifndef JLINEALGCPP_MATRIX_H
#define JLINEALGCPP_MATRIX_H

#include <format>
#include <stdexcept>

namespace JLinAlg {
    using element_t = double;
    using element_reference = element_t&;

    class MatrixArray {
        size_t M = 0;
        size_t N = 0;
        element_t *elements = nullptr;

        bool make_all_zeroes();

    public:
        MatrixArray() = delete;

        /**
         * Constructs empty nxn matrix.
         */
        explicit MatrixArray(const size_t n) {
            M = n;
            N = n;
            elements = new element_t[M*N];

            make_all_zeroes();
        }

        /**
         * Constructs empty mxn matrix.
         */
        explicit MatrixArray(const size_t m, const size_t n) {
            M = m;
            N = n;
            elements = new element_t[M*N];
            make_all_zeroes();
        }

        MatrixArray(const MatrixArray &other) {
            this->M = other.M;
            this->N = other.N;

            elements = new element_t[M*N];
            for (size_t m = 0; m < M; m++) {
                for (size_t n = 0; n < N; n++) {
                    (*this)[m, n] = other[m, n];
                }
            }
        }

        ~MatrixArray() {
            delete[] elements;
            elements = nullptr;
        }


        /**
         * Returns a copy of matrix element at position m, n.
         */
        element_t operator[](size_t m, size_t n) const;
        /**
         * Returns a reference to matrix element at position m, n.
         */
        element_reference operator[](size_t m, size_t n);


        [[nodiscard]] size_t m() const { return M; }
        [[nodiscard]] size_t n() const { return N; }
    };

    class Matrix {
        MatrixArray elements = MatrixArray(1);

        bool make_identity();

    public:
        /**
         * Constructs empty 2x2 matrix.
         */
        Matrix() = delete;

        /**
         * Constructs empty nxn matrix.
         */
        explicit Matrix(const size_t n) {
            elements = MatrixArray(n);
            make_identity();
        }

        /**
         * Constructs empty mxn matrix.
         */
        explicit Matrix(const size_t m, const size_t n) {
            elements = MatrixArray(m, n);
            make_identity();
        }

        Matrix(const Matrix &other) {
            this->elements = other.elements;
        }


        /**
         * Returns a copy of matrix element at position m, n.
         */
        element_t operator[](size_t m, size_t n) const;
        /**
         * Returns a reference to matrix element at position m, n.
         */
        element_reference operator[](size_t m, size_t n);

        /**
         * Performs matrix addition.\n
         * Returns a new matrix.
         */
        Matrix operator+(const Matrix &other) const;
        Matrix operator-(const Matrix &other) const;

        Matrix operator*(const Matrix &other) const;


        Matrix& echelon();


        Matrix& swap_rows(size_t r1, size_t r2);


        [[nodiscard]] size_t rows() const { return elements.m(); }
        [[nodiscard]] size_t columns() const { return elements.n(); }
    };


    inline Matrix operator+=(Matrix L, const Matrix &R) {
        if (L.rows() != R.rows() || L.columns() != R.columns())
            throw std::invalid_argument(std::format(
                "Matrices have different forms.\nLeft matrix: {}x{}\nRight matrix: {}x{}",
                L.rows(), L.columns(),
                R.rows(), R.columns()
            ));

        for (size_t m = 0; m < L.rows(); m++) {
            for (size_t n = 0; n < L.columns(); n++) {
                L[m, n] = L[m, n] + R[m, n];
            }
        }

        return L;
    }
    inline Matrix operator-=(Matrix L, const Matrix &R) {
        if (L.rows() != R.rows() || L.columns() != R.columns())
            throw std::invalid_argument(std::format(
                "Matrices have different forms.\nLeft matrix: {}x{}\nRight matrix: {}x{}",
                L.rows(), L.columns(),
                R.rows(), R.columns()
            ));

        for (size_t m = 0; m < L.rows(); m++) {
            for (size_t n = 0; n < L.columns(); n++) {
                L[m, n] = L[m, n] - R[m, n];
            }
        }

        return L;
    }
} // JLinAlg

#endif //JLINEALGCPP_MATRIX_H
