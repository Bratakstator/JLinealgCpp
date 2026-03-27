//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    int Matrix::swap_rows(const int r1, const int r2) {
        if (!(r1 == -1 || r2 == -1 || r1 == r2)) return 1;
        row_space_.swap(r1, r2);
        return 0;
    }

    int Matrix::get_non_zero_row_in_col(const int col, const int current_row) {
        for (int row = current_row; row < rows(); row++) {
            if ((*this)[row, col] != 0) return row;
        }
        return -1;
    }

    int Matrix::get_first_non_zero_col(const int row) {
        for (int col = 0; col < columns(); col++) {
            if ((*this)[row, col] != 0) return col;
        }
        return -1;
    }

    Matrix Matrix::row_echelon(const bool pivots_must_be_one) {
        if (echelons_.REF_calculated && echelons_.REF_with_pivots_eq_one == pivots_must_be_one) return Matrix(echelons_.REF);
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
                            if (ech.swap_rows(pivot, ech.get_non_zero_row_in_col(col, pivot)) == 0) {
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

        echelons_.REF_calculated = true;
        echelons_.REF_with_pivots_eq_one = pivots_must_be_one;
        echelons_.REF = ech.row_space_;
        return ech;
    }

    Matrix Matrix::reduced_row_echelon() {
        if (echelons_.RREF_calculated) return Matrix(echelons_.RREF);
        if (identity_ == 1) return *this;
        Matrix ech = row_echelon(true);

        if (diagonal_ != 1) {
            for (size_t pivot_complement = 0; pivot_complement < ech.rows(); pivot_complement++) {
                auto pivot = (ech.rows() - 1) - pivot_complement;
                auto pivot_col = ech.get_first_non_zero_col(pivot);
                if (pivot_col == -1) continue;

                for (size_t row_complement = 0; row_complement < pivot; row_complement++) {
                    auto row = (pivot - 1) - row_complement;
                    component_t factor = ech[row, pivot_col];
                    for (size_t col = pivot_col; col < ech.columns(); col++) {
                        ech[row, col] -= factor * ech[pivot, col];
                    }
                }
            }
        }

        echelons_.RREF_calculated = true;
        echelons_.RREF = ech.row_space_;
        return ech;
    }
}
