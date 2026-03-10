//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_MATRIX_H
#define JLINEALGCPP_MATRIX_H
#include "Vector.h"

namespace Objects {
    class Matrix {
    public:
        Vector *vecs_;
        int rows_;
        int cols_;

    public:
        Matrix(const int rows, const int cols) : rows_(rows), cols_(cols) {
            vecs_ = new Vector[rows];
            for (int i = 0; i < rows; i++) {
                vecs_[i] = Vector(cols);
            }
        }
        ~Matrix() {
            delete vecs_;
        }


        Vector& operator[](const int n) { //NOLINT
            return vecs_[n];
        }


        std::pair<Matrix, Matrix> LU() {
            Matrix L(rows_, cols_);
            Matrix U(rows_, cols_);

            for (int row = 1; row < rows_; row++) {
                L[row][0] = (*this)[row][0] / (*this)[0][0];
            }

            for (int row = 2; row < rows_; row++) {
                for (int col = 1; col < cols_; col++) {
                    L[row][col] = ((*this)[row][col] - L[row][col-1] * (*this)[0][col]) / ((*this)[row-1][col] - L[row-1][0] * (*this)[0][col]);
                }
            }

            return std::make_pair(L, U);
        }

        void print() const {
            for (int row = 0; row < rows_; row++) {
                for (int col = 0; col < cols_; col++) {
                    auto a = vecs_[row][col];
                    std::cout << a << " ";
                }
                std::cout << "\n";
            }
        }
    };
} // Objects

#endif //JLINEALGCPP_MATRIX_H