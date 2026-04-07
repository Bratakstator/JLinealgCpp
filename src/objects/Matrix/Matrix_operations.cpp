//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    code_t Matrix::swap_rows(const size_t r1, const size_t r2, const code_t code) const {
        if (code != 0) return 128 + code;
        if (r1 == r2) return 1;

        row_space_.swap(r1, r2);

        if (cache_.det.valid) cache_.det.is *= (-1);
        cache_.REF.valid = false;
        cache_.RREF.valid = false;

        return 0;
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
        if (auto [i, v] = cache_.REF_force_eq_one;
            cache_.REF.valid && ((i && v) == pivots_must_be_one)) return Matrix(cache_.REF.is);
        if (cache_.identity.is && cache_.identity.valid) return *this;

        Matrix ech(row_space_);
        if (auto [is, valid] = cache_.diagonal; (is && valid) && pivots_must_be_one) {
            for (size_t p = 0; p < ech.rows(); p++) ech[p, p] = 1;
        }
        else {
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

        cache_.REF.valid = true;
        cache_.REF_force_eq_one.is = cache_.REF_force_eq_one.valid = pivots_must_be_one;
        cache_.REF.is = ech.row_space_;
        return ech;
    }

    Matrix Matrix::reduced_row_echelon() const {
        if (cache_.RREF.valid) return Matrix(cache_.RREF.is);
        if (auto [is, valid] = cache_.identity; is && valid) return *this;

        Matrix ech = row_echelon(true);
        if (auto [is, valid] = cache_.diagonal; !(is && valid)) {
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

        cache_.RREF.valid = true;
        cache_.RREF.is = ech.row_space_;
        return ech;
    }

    Matrix Matrix::invert() const {
        if (cache_.inverse.valid) return Matrix(cache_.inverse.is);

        if (!cache_.invertible.valid) {
            if (rows() != columns()) {
                cache_.invertible = {false, true};
                throw std::invalid_argument("Matrix is not square.");
            }
            cache_.invertible = {true, true};
        }

        Matrix I(rows());
        std::cout << "\nMatrix::invert() | Printing I\n";
        I.print();
        if (cache_.diagonal.valid && cache_.diagonal.is) {
            if (cache_.identity.valid && cache_.identity.is) return *this;
            for (size_t pivot = 0; pivot < rows(); pivot++) I[pivot, pivot] = 1/(*this)[pivot, pivot];
        }
        else {
            Matrix A(rows(), 2 * rows());
            Matrix current = (*this);
            std::cout << "A[row, col + i * (A.columns()/2)] = current[row, col]\n";
            for (size_t i = 0; i < 2; i++) {
                std::cout << "Current Matrix:\n";
                current.print();
                for (size_t row = 0; row < A.rows(); row++) {
                    for (size_t col = 0; col < A.columns()/2; col++) {
                        A[row, col + i * (A.columns()/2)] = current[row, col];
                        std::cout << "A[" << row << ", " << col << " + " << i << " * ";
                        std::cout << A.columns() << "/2] = current[" << row << ", " << col << "] => A[";
                        std::cout << row << ", " << col + i * (A.columns()/2) << "] = current[" << row << ", ";
                        std::cout << col << "] =>\n";

                        std::cout << A[row, col+i*(A.columns()/2)] << " = " << current[row, col] << ":\n";
                        A.print();
                    }
                }
                current = Matrix(rows());
            }

            std::cout << "A before reduced row echelon:\n";
            A.print();
            A = A.reduced_row_echelon();
            std::cout << "A after reduced row echelon:\n";
            A.print();

            for (size_t row = 0; row < rows(); row++) {
                for (size_t col = 0; col < A.columns()/2; col++) {
                    I[row, col] = A[row, col + A.columns()/2];
                }
            }
        }

        cache_.inverse.is = I.row_space_;
        cache_.inverse.valid = true;
        return I;
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
