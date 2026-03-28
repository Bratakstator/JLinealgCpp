//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    code_t Matrix::swap_rows(const size_t r1, const size_t r2, const code_t code) {
        if (code != 0) return 128 + code;
        if (r1 == r2) return 1;

        row_space_.swap(r1, r2);
        return 0;
    }

    Code Matrix::get_non_zero_row_in_col(const size_t col, const size_t current_row) {
        for (size_t row = current_row; row < rows(); row++) {
            if ((*this)[row, col] != 0) return {row, 0};
        }
        return {current_row, -1};
    }

    Code Matrix::get_first_non_zero_col(const size_t row) {
        for (size_t col = 0; col < columns(); col++) {
            if ((*this)[row, col] != 0) return {col, 0};
        }
        return {0, -1};
    }

    Matrix Matrix::row_echelon(const bool pivots_must_be_one) {
        if (echelons_.REF_valid && echelons_.REF_with_pivots_eq_one == pivots_must_be_one) return Matrix(echelons_.REF);
        if (identity_ == 1) return *this;
        Matrix ech(row_space_);
        if (diagonal_ == 1 && pivots_must_be_one) {
            for (size_t p = 0; p < ech.rows(); p++) ech[p, p] = 1;
        }
        else if (diagonal_ != 1) {
            for (size_t pivot = 0; pivot < ech.rows(); pivot++) {
                auto pivot_col = pivot;

                for (size_t row = pivot; row < ech.rows(); row++) {
                    component_t pivot_val = ech[pivot, pivot_col];
                    component_t factor = ech[row, pivot_col];
                    if (pivot_val != 0) factor /= pivot_val;

                    for (size_t col = pivot_col; col < ech.columns(); col++) {
                        if (pivot_val == 0 && ech[pivot, col] == 0) {
                            auto [index, code] = ech.get_non_zero_row_in_col(col, pivot);
                            if (ech.swap_rows(pivot, index, code) == 0) {
                                pivot_col = col;
                                pivot_val = ech[pivot, pivot_col];
                            }
                            else continue;
                        }
                        else if (pivot_val == 0 && ech[pivot, col] != 0) {
                            pivot_col = col;
                            pivot_val = ech[pivot, pivot_col];
                        }

                        if (row == pivot && pivots_must_be_one) ech[row, col] /= pivot_val;
                        else if (row != pivot) ech[row, col] -= factor * ech[pivot, col];
                    }
                    if (pivot_val == 0) pivot = ech.rows();
                }
            }
        }

        echelons_.REF_valid = true;
        echelons_.REF_with_pivots_eq_one = pivots_must_be_one;
        echelons_.REF = ech.row_space_;
        return ech;
    }

    Matrix Matrix::reduced_row_echelon() {
        if (echelons_.RREF_valid) return Matrix(echelons_.RREF);
        if (identity_ == 1) return *this;
        Matrix ech = row_echelon(true);

        if (diagonal_ != 1) {
            for (size_t pivot_complement = 0; pivot_complement < ech.rows(); pivot_complement++) {
                const auto pivot = (ech.rows() - 1) - pivot_complement;
                auto [pivot_col, code] = ech.get_first_non_zero_col(pivot);
                if (code == -1) continue;

                for (size_t row_complement = 0; row_complement < pivot; row_complement++) {
                    const auto row = (pivot - 1) - row_complement;
                    const component_t factor = ech[row, pivot_col];

                    for (size_t col = pivot_col; col < ech.columns(); col++) {
                        ech[row, col] -= factor * ech[pivot, col];
                    }
                }
            }
        }

        echelons_.RREF_valid = true;
        echelons_.RREF = ech.row_space_;
        return ech;
    }
}
