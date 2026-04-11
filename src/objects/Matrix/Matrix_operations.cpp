//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"

namespace Objects {
    code_t Matrix::swap_rows(const size_t r1, const size_t r2, const code_t code) { // NOLINT
        if (code != 0) return 128 + code;
        if (r1 == r2) return 1;

        row_space_.swap(r1, r2);

        if (cache_.det.valid) cache_.det.is *= (-1);
        cache_.REF.valid = false;
        cache_.RREF.valid = false;

        return 0;
    }

    void Matrix::do_row_echelon(const AugmentedMatrix &AB, const bool pivots_must_be_one) {
        auto [is, valid] = AB.A().cache_.REF_force_eq_one; // NOLINT
        if (AB.A().cache_.REF.valid && (is && valid) == pivots_must_be_one) return;

        if (AB.A().cache_.identity.valid && AB.A().cache_.identity.is) return;

        is = AB.A().cache_.diagonal.is;
        valid = AB.A().cache_.diagonal.valid;
        if ((is && valid) && pivots_must_be_one) {
            for (size_t pivot = 0; pivot < AB.A().rows(); pivot++) AB[pivot] /= AB.A()[pivot, pivot];
            return;
        }

        for (size_t pivot = 0; pivot < AB.A().rows(); pivot++) {
            auto pivot_col = pivot;

            for (size_t row = pivot; row < AB.A().rows(); row++) {
                component_t pivot_val = AB.A()[pivot, pivot_col];
                component_t factor = AB.A()[row, pivot_col];
                if (pivot_val != 0) factor /= pivot_val;

                for (size_t col = pivot_col; col < AB.A().columns(); col++) {
                    if (pivot_val == 0 && AB.A()[pivot, col] == 0) {
                        auto [index, code] = AB.A().get_non_zero_row_in_col(col, pivot);
                        if (AB.swap_rows(pivot, index, code) == 0) {
                            pivot_col = col;
                            pivot_val = AB.A()[pivot, pivot_col];
                        }
                    }
                    else if (pivot_val == 0 && AB.A()[pivot, col] != 0) {
                        pivot_col = col;
                        pivot_val = AB.A()[pivot, pivot_col];
                    }
                }

                if (row == pivot && pivots_must_be_one) AB[row] /= pivot_val;
                else if (row != pivot) {
                    AB[row] -= factor * AB[pivot];
                }

                if (pivot_val == 0) pivot = AB.A().rows();
            }
        }
    }

    void Matrix::do_reduced_row_echelon(const AugmentedMatrix &AB) {
        if (AB.A().cache_.RREF.valid) return;

        auto [is, valid] = AB.A().cache_.identity;
        if (is && valid) return;

        do_row_echelon(AB, true);

        is = AB.A().cache_.diagonal.is;
        valid = AB.A().cache_.diagonal.valid;
        if (is && valid) return;

        for (size_t pivot_complement = 0; pivot_complement < AB.A().rows(); pivot_complement++) {
            const auto pivot = (AB.A().rows() - 1) - pivot_complement;
            auto [pivot_col, code] = AB.A().get_first_non_zero_col(pivot);
            if (code == -1) continue;

            for (size_t row_complement = 0; row_complement < pivot; row_complement++) {
                const auto row = (pivot - 1) - row_complement;
                component_t factor = AB.A()[row, pivot_col];

                if (row != pivot) {
                    AB[row] -= factor * AB[pivot];
                }
            }
        }
    }


    Code Matrix::get_non_zero_row_in_col(const size_t col, const size_t current_row) const {
        for (size_t row = current_row; row < rows(); row++) {
            if ((*this)[row, col] != 0) return {row, 0};
        }
        return {current_row, -1};
    }

    Code Matrix::get_first_non_zero_col(const size_t row) const {
        for (size_t col = 0; col < columns(); col++) {
            if ((*this)[row, col] != 0) return {col, 0};
        }
        return {0, -1};
    }

    Matrix Matrix::row_echelon(const bool pivots_must_be_one) const {
        Matrix A(row_space_);
        Matrix B(rows(), 1);
        AugmentedMatrix AB(A, B);

        do_row_echelon(AB, pivots_must_be_one);

        cache_.REF.is = A.row_space_;
        cache_.REF.valid = true;

        cache_.REF_force_eq_one.is = pivots_must_be_one;
        cache_.REF_force_eq_one.valid = true;

        return A;
    }

    Matrix Matrix::reduced_row_echelon() const {
        Matrix A = row_echelon(true);
        Matrix B(rows(), 1);
        AugmentedMatrix AB(A, B);

        do_reduced_row_echelon(AB);

        cache_.RREF.is = A.row_space_;
        cache_.RREF.valid = true;

        return A;
    }

    Matrix Matrix::invert() const {
        if (cache_.inverse.valid) return Matrix(cache_.inverse.is);

        if (!invertible()) {
            throw std::invalid_argument("Matrix is not invertible.");
        }

        Matrix A(row_space_);
        Matrix B(rows());
        AugmentedMatrix AB(A, B);

        do_reduced_row_echelon(AB);

        cache_.inverse.is = B.row_space_;
        cache_.inverse.valid = true;

        return B;
    }

    Matrix Matrix::transpose() const {
        Matrix T(columns(), rows());

        for (int row = 0; row < T.rows(); row++) {
            for (int col = 0; col < T.columns(); col++) {
                T[row, col] = (*this)[col, row];
            }
        }

        return T;
    }
}
