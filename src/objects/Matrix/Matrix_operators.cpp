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

    Matrix Matrix::operator*(const Matrix &other) {
        if (identity() && other.identity()) return *this;
        if (identity()) return {other};
        if (other.identity()) return {*this};

        if (columns() != other.rows()) {
            throw std::invalid_argument(
                std::string("Matrix A columns and matrix B rows does not match.\n")
            );
        }

        Matrix C(Span(rows(), other.columns()));
        for (size_t row = 0; row < rows(); row++) {
            for (size_t col = 0; col < other.columns(); col++) {
                for (size_t cxr = 0; cxr < columns(); cxr++) {
                    C[row, col] += (*this)[row, cxr] * other[cxr, col];
                }
            }
        }

        return C;
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