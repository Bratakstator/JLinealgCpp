//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    DoubleProxy Matrix::operator[](int m, int n) {
        return span_[m][n];
    }
    double Matrix::operator[](const int m, const int n) const {
        return span_[m][n];
    }

    Matrix Matrix::operator*(const Matrix &other) {
        if (identity() && other.identity()) return {};
        if (identity()) return {other};
        if (other.identity()) return {*this};

        if (columns() != other.rows()) {
            throw std::invalid_argument(
                std::string("Matrix A columns and matrix B rows does not match.\n")
            );
        }

        Matrix C(Span(rows(), other.columns()));
        for (int m = 0; m < rows(); m++) {
            for (int n = 0; n < other.columns(); n++) {
                for (int cxr = 0; cxr < columns(); cxr++) {
                    C[m, n] += (*this)[m, cxr] * other[cxr, m];
                }
            }
        }

        return C;
    }
} // Objects