//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_MATRIX_H
#define JLINEALGCPP_MATRIX_H

#include "Span.h"

namespace Objects {
    class Matrix {
        Span span_;

        int identity_ = 1; // -1: not calculated, 0: false, 1: true

    public:
        Matrix() = default;
        explicit Matrix(const Span &span) {
            identity_ = -1;
            span_ = span;
        }
        explicit Matrix(const std::initializer_list<Vector> span) {
            identity_ = -1;
            span_ = Span(span);
        }
        Matrix(const Matrix &other) {
            identity_ = other.identity_;
            span_ = other.span_;
        }


        DoubleProxy operator[](int m, int n) { // NOLINT
            return span_[m][n];
        }
        double operator[](const int m, const int n) const {
            return span_[m][n];
        }

        Matrix operator*(const Matrix &other) { // NOLINT
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

            return *this;
        }


        [[nodiscard]] int rows() const { return span_.size(); }
        [[nodiscard]] int columns() const { return span_.room(); }


        [[nodiscard]] bool identity() {
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
        [[nodiscard]] bool identity() const {
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
    };
} // Objects

#endif //JLINEALGCPP_MATRIX_H