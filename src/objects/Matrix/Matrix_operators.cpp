//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    ComponentPPProxy Matrix::operator[](const size_t m, const size_t n) {
        auto proxy = row_space_[m][n];
        return {proxy, cache_};
    }
    double Matrix::operator[](const size_t m, const size_t n) const {
        return row_space_[m][n];
    }


    Matrix& Matrix::operator+=(const Matrix &B) {
        if (!(rows() == B.rows() && columns() == B.columns())) throw std::invalid_argument("Matrices not of equal size.");

        for (size_t row = 0; row < rows(); row ++) {
            for (size_t col = 0; col < columns(); col++) {
                (*this)[row, col] += B[row, col];
            }
        }
        return *this;
    }

    Matrix& Matrix::operator-=(const Matrix &B) {
        if (!(rows() == B.rows() && columns() == B.columns())) throw std::invalid_argument("Matrices not of equal size.");

        for (size_t row = 0; row < rows(); row ++) {
            for (size_t col = 0; col < columns(); col++) {
                (*this)[row, col] -= B[row, col];
            }
        }
        return *this;
    }

    Matrix& Matrix::operator*=(const Matrix &B) {
        if (B.identity()) return *this;
        if (identity()) {
            (*this) = B;
            return *this;
        }

        if (columns() != B.rows()) throw std::invalid_argument("Matrices not of compatible sizes.");

        Matrix A(*this);
        (*this) = Matrix(rows(), B.columns());

        for (size_t row = 0; row < rows(); row++) {
            for (size_t col = 0; col < columns(); col++) {
                for (size_t cxr = 0; cxr < A.columns(); cxr++) {
                    (*this)[row, col] += A[row, cxr] * B[cxr, col];
                }
            }
        }
        return *this;
    }

    Matrix& Matrix::operator*=(const component_t c) {
        for (size_t row = 0; row < rows(); row++) {
            for (size_t col = 0; col < columns(); col++) {
                (*this)[row, col] *= c;
            }
        }
        return *this;
    }

    Matrix& Matrix::operator/=(const component_t c) {
        for (size_t row = 0; row < rows(); row++) {
            for (size_t col = 0; col < columns(); col++) {
                (*this)[row, col] /= c;
            }
        }
        return *this;
    }


    Vector Matrix::get_column(const size_t n) const {
        if (n >= columns()) throw std::out_of_range("Column index out of range");

        Vector v(rows());
        for (size_t row = 0; row < rows(); row++) v[row] = (*this)[row, n];
        return v;
    }

    Vector Matrix::get_row(const size_t m) const {
        if (m >= rows()) throw std::out_of_range("Row index out of range");

        return row_space_[m];
    }
} // Objects