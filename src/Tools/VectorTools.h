//
// Created by JoakimKB on 27.03.2026.
//

#ifndef JLINEALGCPP_VECTORTOOLS_H
#define JLINEALGCPP_VECTORTOOLS_H

#include "../objects_OLD/Matrix/Matrix.h"

namespace Tools::VectorTools {
    using component_t = Objects_OLD::component_t;

    inline component_t inner_product(const Objects_OLD::Vector &u, const Objects_OLD::Vector &v) {
        if (u.dimension() != v.dimension()) throw std::invalid_argument("Vector sizes does not match.");

        component_t sum = 0;
        for (size_t i = 0; i < u.dimension(); i++) sum += u[i] * v[i];
        return sum;
    }

    inline Objects_OLD::Matrix outer_product(const Objects_OLD::Vector &u, const Objects_OLD::Vector &v) {
        Objects_OLD::Matrix A(u.dimension(), v.dimension());
        for (size_t row = 0; row < A.rows(); row++) {
            for (size_t col = 0; col < A.columns(); col++) {
                A[row, col] = u[row] * v[col];
            }
        }
        return A;
    }

    inline Objects_OLD::Matrix projection_matrix(const Objects_OLD::Vector &a) {
        return outer_product(a, a) / inner_product(a, a);
    }

    inline Objects_OLD::Vector project(const Objects_OLD::Vector &a, const Objects_OLD::Vector &b) {
        return (inner_product(a, b)/inner_product(a, a)) * a;
    }
}

#endif //JLINEALGCPP_VECTORTOOLS_H
