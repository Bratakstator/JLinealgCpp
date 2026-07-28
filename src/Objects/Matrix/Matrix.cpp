//
// Created by JoakimKB on 13.04.2026.
//

#include "Matrix.h"

namespace JLinAlg {
    /*
     * MatrixArray implementations
     */
    // ReSharper disable once CppMemberFunctionMayBeConst
    bool MatrixArray::make_all_zeroes() {
        for (size_t i = 0; i < M*N; i++) elements[i] = 0;

        return true;
    }

    element_t MatrixArray::operator[](const size_t m, const size_t n) const {
        if (elements == nullptr) throw std::invalid_argument("Array is nullptr.");

        if (m >= M) throw std::out_of_range("Requested row out of range.");
        if (n >= N) throw std::out_of_range("Requested column out of range.");

        return elements[N*m + n]; //NOLINT
    }

    element_t& MatrixArray::operator[](const size_t m, const size_t n) {
        if (elements == nullptr) throw std::invalid_argument("Array is nullptr.");

        if (m >= M) throw std::out_of_range("Requested row out of range.");
        if (n >= N) throw std::out_of_range("Requested column out of range.");

        return elements[N*m + n]; //NOLINT
    }

    /*
     * Matrix implementations
     */
    // ReSharper disable once CppMemberFunctionMayBeConst
    bool Matrix::make_identity() {
        if (rows() != columns()) return false;

        for (size_t i = 0; i < rows(); i++) elements[rows(), rows()] = 1;

        return true;
    }

    element_t Matrix::operator[](const size_t m, const size_t n) const { return elements[m, n]; }
    element_reference Matrix::operator[](const size_t m, const size_t n) { return elements[m, n]; }

    Matrix Matrix::operator+(const Matrix &other) const { return (*this) += other; }
    Matrix Matrix::operator-(const Matrix &other) const { return (*this) -= other; }

    Matrix Matrix::operator*(const Matrix &other) const {
        if (this->columns() != other.rows())
            throw std::invalid_argument("Column of left matrix does not match the rows of the right matrix.");

        Matrix C(this->rows(), other.columns());
        for (size_t i = 0; i < this->rows(); i++) {
            for (size_t j = 0; j < other.columns(); j++) {
                for (size_t k = 0; k < this->columns(); k++) {
                    C[i, j] = (*this)[i, k] * other[k, j];
                }
            }
        }

        return C;
    }


    Matrix& Matrix::echelon() {
        for (size_t p = 0; p < rows(); p++) {
            auto pr = p;
            auto pc = p;
            element_t pivot_val = (*this)[pr, pc];

            if (pivot_val == 0) {
                for (size_t c = pc; c < columns(); c++) {
                    for (size_t r = pr+1; r < rows(); r++) {
                        if ((*this)[r, c] != 0) {
                            swap_rows(pr, r);
                            pc = c;
                            pivot_val = (*this)[pr, pc];
                            break;
                        }
                    }
                    if (pivot_val != 0) break;
                }
            }

            for (size_t r = pr; r < rows(); r++) {
                for (size_t c = pc; c < columns(); c++) {
                    if (r == pr) (*this)[r, c] /= pivot_val;
                    else (*this)[r, c] -= (*this)[r, c] * (*this)[r-1, c];
                }
            }
        }

        return *this;
    }


    Matrix& Matrix::swap_rows(const size_t r1, const size_t r2) {
        if (r1 >= rows() || r2 >= rows()) throw std::invalid_argument("Given rows out of range.");

        if (r1 != r2) {
            element_t old_r1 = 0;
            for (size_t c = 0; c < columns(); c++) {
                old_r1 = (*this)[r1, c];
                (*this)[r1, c] = (*this)[r2, c];
                (*this)[r2, c] = old_r1;
            }
        }
        return (*this);
    }
} // JLinAlg