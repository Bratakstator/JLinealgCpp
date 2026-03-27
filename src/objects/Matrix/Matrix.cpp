//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"

#include <iomanip>

#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {

    Matrix::Matrix() {
        span_ = Span({
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        });
    }

    Matrix::Matrix(const Span &span) {
        identity_ = -1;
        span_ = span;
    }

    Matrix::Matrix(const std::initializer_list<Vector> span) {
        identity_ = -1;
        span_ = Span(span);
    }

    Matrix::Matrix(const Matrix &other) {
        identity_ = other.identity_;
        span_ = other.span_;
    }


    double Matrix::determinant() {
        if (rows() != columns()) return 0;
        if (determinant_calculated) return determinant_;
        if (!echelons_.REF_calculated || echelons_.REF_with_pivots_eq_one) row_echelon();

        Matrix ech(echelons_.REF);
        determinant_ = 1;
        for (int p = 0; p < rows(); p++) determinant_ *= ech[p, p];
        determinant_calculated = true;

        return determinant_;
    }


    [[nodiscard]] bool Matrix::identity() {
        if (identity_ != -1) return identity_;
        if (span_.size() != span_.room()) identity_ = 0;
        else {
            identity_ = 1;
            for (int m = 0; m < span_.size(); m++) {
                for (int n = 0; n < span_.room(); n++) {
                    const double val = (*this)[m, n]; // NOLINT
                    if ((m == n && val != 1) || val != 0) {
                        identity_ = 0;
                        break;
                    }
                }
                if (identity_ == 0) break;
            }
        }
        return identity_;
    }
    [[nodiscard]] bool Matrix::identity() const {
        if (identity_ != -1) return identity_;
        if (span_.size() != span_.room()) return false;

        for (int m = 0; m < span_.size(); m++) {
            for (int n = 0; n < span_.room(); n++) {
                const double val = (*this)[m, n]; // NOLINT
                if ((m == n && val != 1) || val != 0) return false;
            }
        }
        return true;
    }


    [[nodiscard]] int Matrix::rows() const { return span_.size(); }
    [[nodiscard]] int Matrix::columns() const { return span_.room(); }


    void Matrix::print() const {
        std::cout << std::fixed << std::setprecision(2);

        for (int row = 0; row < rows(); row++) {
            for (int col = 0; col < columns(); col++) {
                std::cout << (*this)[row, col] << "  ";
            }
            std::cout << "\n";
        }
    }
} // Objects