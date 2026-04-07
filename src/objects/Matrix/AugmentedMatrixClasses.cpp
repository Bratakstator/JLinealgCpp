//
// Created by JoakimKB on 08.04.2026.
//

#include "Matrix.h"

namespace Objects {
    Matrix& AugmentedMatrix::A() const { return A_; }
    Matrix& AugmentedMatrix::B() const { return B_; }

    AugmentedMatrixVectorPair<VectorProxy&> AugmentedMatrix::operator[](const size_t i) {
        auto rowA = A_.row_space_[i];
        auto rowB = B_.row_space_[i];

        return {rowA, rowB};
    }
    AugmentedMatrixVectorPair<VectorProxy> AugmentedMatrix::operator[](const size_t i) const {
        return {A_.row_space_[i], B_.row_space_[i]};
    }

    code_t AugmentedMatrix::swap_rows(const size_t r1, const size_t r2, const code_t code) const {
        B_.swap_rows(r1, r2, code);
        return A_.swap_rows(r1, r2, code);
    }


    template<VectorProxyOnly T>
    AugmentedMatrixVectorPair<T>::operator Vector() const { return v1_; }

    template<VectorProxyOnly T>
    AugmentedMatrixVectorPair<T> AugmentedMatrixVectorPair<T>::operator-=(AugmentedMatrixVectorPair v) {
        v1_ -= v.v1_;
        v2_ -= v.v2_;
        return *this;
    }

    template<VectorProxyOnly T>
    AugmentedMatrixVectorPair<T> AugmentedMatrixVectorPair<T>::operator/=(const component_t component) {
        v1_ /= component;
        v2_ /= component;
        return *this;
    }

    template<VectorProxyOnly T>
    AugmentedMatrixVectorPair<T> AugmentedMatrixVectorPair<T>::operator*=(const component_t component) {
        v1_ *= component;
        v2_ *= component;
        return *this;
    }
}
