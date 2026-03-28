//
// Created by Joakim on 22.03.2026.
//

#include <iomanip>

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    Matrix::Matrix(const size_t n) {
        row_space_ = Span(n, n);

        for (size_t p = 0; p < rows(); p++) row_space_[p][p] = 1;
        identity_ = 1;
        diagonal_ = 1;
        determinant_ = 1;
        determinant_calculated = true;
    }

    Matrix::Matrix(const size_t m, const size_t n) {
        row_space_ = Span(m, n);
    }

    Matrix::Matrix(const Span &row_space) {
        row_space_ = row_space;
    }

    Matrix::Matrix(const std::initializer_list<Vector> row_space) {
        row_space_ = Span(row_space);
    }

    Matrix::Matrix(const Matrix &other) {
        row_space_ = other.row_space_;
        echelons_ = other.echelons_;

        identity_ = other.identity_;
        diagonal_ = other.diagonal_;
        determinant_ = other.determinant_;
        determinant_calculated = other.determinant_calculated;
    }


    det_t Matrix::determinant() {
        if (rows() != columns()) return 0;
        if (determinant_calculated) return determinant_;
        if (diagonal_ == 1) {
            determinant_ = 1;
            for (size_t p = 0; p < rows(); p++) determinant_ *= (*this)[p, p];
            determinant_calculated = true;
            return determinant_;
        }
        if (!echelons_.REF_valid || echelons_.REF_with_pivots_eq_one) row_echelon();

        Matrix ech(echelons_.REF);
        determinant_ = 1;
        for (size_t p = 0; p < rows(); p++) determinant_ *= ech[p, p];
        determinant_calculated = true;

        return determinant_;
    }


    [[nodiscard]] bool Matrix::identity() {
        if (identity_ != -1) return identity_;
        if (row_space_.rank() != row_space_.vector_dimension()) identity_ = 0;
        else {
            identity_ = 1;
            for (size_t m = 0; m < row_space_.size(); m++) {
                for (size_t n = 0; n < row_space_.vector_dimension(); n++) {
                    const component_t component = (*this)[m, n]; // NOLINT
                    if ((m == n && component != 1) || component != 0) {
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
        if (row_space_.rank() != row_space_.vector_dimension()) return false;

        for (size_t m = 0; m < row_space_.size(); m++) {
            for (size_t n = 0; n < row_space_.vector_dimension(); n++) {
                const component_t component = (*this)[m, n]; // NOLINT
                if ((m == n && component != 1) || component != 0) return false;
            }
        }
        return true;
    }


    [[nodiscard]] size_t Matrix::rows() const { return row_space_.size(); }
    [[nodiscard]] size_t Matrix::columns() const { return row_space_.vector_dimension(); }


    void Matrix::print() const {
        std::cout << std::fixed << std::setprecision(2);

        for (size_t row = 0; row < rows(); row++) {
            for (size_t col = 0; col < columns(); col++) {
                std::cout << (*this)[row, col] << "  ";
            }
            std::cout << "\n";
        }
    }
} // Objects