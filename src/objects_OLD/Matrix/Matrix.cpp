//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"

namespace Objects_OLD {
    Matrix::Matrix(const size_t n) {
        row_space_ = Span(n, n);

        for (size_t p = 0; p < rows(); p++) row_space_[p][p] = 1;

        cache_ = MatrixCache(
            CacheInstance(true, true),
            CacheInstance(true, true),
            CacheInstance(true, true),
            CacheInstance(false, true),
            CacheInstance(true, true),

            CacheInstance(true, true),
            CacheInstance(true, true),

            CacheInstance(true, true),
            CacheInstance(true, true),

            CacheInstance<det_t>(1, true),

            CacheInstance(Span(row_space_), true),
            CacheInstance(false, true),
            CacheInstance(Span(row_space_), true),

            CacheInstance(PLU(Span(), Span(), Span()), false),
            CacheInstance(QR(Span(), Span()), false),
            CacheInstance(Span(row_space_), true),
            CacheInstance(EigenPairs(Vector(), Span()), false)
        );
    }

    Matrix::Matrix(const size_t m, const size_t n) {
        row_space_ = Span(m, n);

        cache_ = MatrixCache(
            CacheInstance(false, false),
            CacheInstance(false, false),
            CacheInstance(false, false),
            CacheInstance(false, false),
            CacheInstance(false, false),

            CacheInstance(false, false),
            CacheInstance(false, false),

            CacheInstance(false, false),
            CacheInstance(false, false),

            CacheInstance<det_t>(0, false),

            CacheInstance(Span(), false),
            CacheInstance(false, false),
            CacheInstance(Span(), false),

            CacheInstance(PLU(Span(), Span(), Span()), false),
            CacheInstance(QR(Span(), Span()), false),
            CacheInstance(Span(), false),
            CacheInstance(EigenPairs(Vector(), Span()), false)
        );
    }

    Matrix::Matrix(const Span &row_space) {
        row_space_ = row_space;

        cache_ = MatrixCache(
            CacheInstance(false, false),
            CacheInstance(false, false),
            CacheInstance(false, false),
            CacheInstance(false, false),
            CacheInstance(false, false),

            CacheInstance(false, false),
            CacheInstance(false, false),

            CacheInstance(false, false),
            CacheInstance(false, false),

            CacheInstance<det_t>(0, false),

            CacheInstance(Span(), false),
            CacheInstance(false, false),
            CacheInstance(Span(), false),

            CacheInstance(PLU(Span(), Span(), Span()), false),
            CacheInstance(QR(Span(), Span()), false),
            CacheInstance(Span(), false),
            CacheInstance(EigenPairs(Vector(), Span()), false)
        );
    }

    Matrix::Matrix(const std::initializer_list<Vector> row_space) {
        row_space_ = Span(row_space);

        cache_ = MatrixCache(
            CacheInstance(false, false),
            CacheInstance(false, false),
            CacheInstance(false, false),
            CacheInstance(false, false),
            CacheInstance(false, false),

            CacheInstance(false, false),
            CacheInstance(false, false),

            CacheInstance(false, false),
            CacheInstance(false, false),

            CacheInstance<det_t>(0, false),

            CacheInstance(Span(), false),
            CacheInstance(false, false),
            CacheInstance(Span(), false),

            CacheInstance(PLU(Span(), Span(), Span()), false),
            CacheInstance(QR(Span(), Span()), false),
            CacheInstance(Span(), false),
            CacheInstance(EigenPairs(Vector(), Span()), false)
        );
    }

    Matrix::Matrix(const Matrix &other) {
        row_space_ = other.row_space_;
        cache_ = other.cache_;
    }


    det_t Matrix::determinant() const {
        if (rows() != columns()) return 0;
        if (cache_.det.valid) return cache_.det.is;

        Matrix A(1);
        if (auto o = cache_.diagonal; o.is && o.valid) A = *this;
        else if (o = cache_.REF_force_eq_one; !cache_.REF.valid || (o.is && o.valid)) A = row_echelon();
        else A = Matrix(cache_.REF.is);

        det_t det = 1;
        for (size_t pivot = 0; pivot < rows(); pivot++) det *= A[pivot, pivot];
        cache_.det.is = det;
        cache_.det.valid = true;

        return cache_.det.is;
    }


    bool Matrix::identity() const {
        if (cache_.identity.valid) return cache_.identity.is;
        if (rank() != columns()) {
            cache_.identity.is = false;
            cache_.identity.valid = true;
        }
        else if (cache_.diagonal.valid) {
            cache_.identity.is = cache_.identity.valid = true;
            for (size_t pivot = 0; pivot < rows(); pivot++) if ((*this)[pivot, pivot] != 1) cache_.identity.is = false;
        }
        else {
            cache_.identity.is = cache_.identity.valid = true;
            for (size_t row = 0; row < rows(); row++) {
                for (size_t col = 0; col < columns(); col++) {
                    if ((row == col && (*this)[row, col] != 1) || (*this)[row, col] != 0) cache_.identity.is = false;
                }
            }
        }
        return cache_.identity.is;
    }


    bool Matrix::invertible() const {
        if (cache_.invertible.valid) return cache_.invertible.is;

        if (rows() != columns()) {
            cache_.invertible = {false, true};
            return false;
        }

        Matrix A(1);
        if (cache_.REF.valid) {
            A = Matrix(cache_.REF.is);
        }
        else {
            A = Matrix(row_space_);
            Matrix B(rows(), 1);
            AugmentedMatrix AB(A, B);

            do_row_echelon(AB);
        }

        for (size_t pivot = 0; pivot < rows(); pivot++) if (A[pivot, pivot] == 0) {
            cache_.invertible = {false, true};
            return false;
        }

        cache_.invertible = {true, true};
        return true;
    }


    size_t Matrix::rows() const { return row_space_.size(); }
    size_t Matrix::columns() const { return row_space_.vector_dimension(); }
    size_t Matrix::rank() const { return row_space_.rank(); }


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