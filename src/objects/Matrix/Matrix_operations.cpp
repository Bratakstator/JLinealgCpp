//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    int Matrix::swap_rows(const int r1, const int r2) {
        if (!(r1 == -1 || r2 == -1 || r1 == r2)) return 1;
        span_.swap(r1, r2);
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

    Matrix Matrix::row_echelon(bool pivots_must_be_one) {
        Matrix ech(span_);

        for (int pivot = 0; pivot < ech.rows(); pivot++) {
            int pivot_col = pivot;

            for (int row = pivot; row < ech.rows(); row++) {
                double pivot_val = ech[pivot, pivot_col];
                double factor = ech[row, pivot_col];
                if (pivot_val == 0) factor /= pivot_val;

                for (int col = pivot_col; col < ech.columns(); col++) {
                    if (pivot_val == 0 && ech[pivot, col] == 0) {
                        if (ech.swap_rows(pivot, get_non_zero_row_in_col(col, pivot)) == 0) {
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
                    else ech[row, col] -= factor * ech[pivot, col];
                }
                if (pivot_val == 0) return ech;
            }
        }

        return ech;
    }

    Matrix Matrix::reduced_row_echelon() {
        Matrix ech = row_echelon(true);

        for (int pivot = ech.rows() - 1; pivot >= 0; pivot--) {
            const int pivot_col = get_first_non_zero_col(pivot);
            if (pivot_col == -1) continue;

            for (int row = pivot - 1; row >= 0; row--) {
                double factor = ech[row, pivot_col];
                for (int col = pivot_col; col >= 0; col--) {
                    ech[row, col] -= factor * ech[pivot, col];
                }
            }
        }

        return ech;
    }
}
