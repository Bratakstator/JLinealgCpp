//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    ComponentProxy Matrix::operator[](const size_t m, const size_t n) {
        return row_space_[m][n];
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
        for (size_t m = 0; m < rows(); m++) {
            for (size_t n = 0; n < other.columns(); n++) {
                for (size_t cxr = 0; cxr < columns(); cxr++) {
                    C[m, n] += (*this)[m, cxr] * other[cxr, n];
                }
            }
        }

        return C;
    }
} // Objects