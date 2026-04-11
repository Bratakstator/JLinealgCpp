//
// Created by JoakimKB on 11.04.2026.
//

#include <vector>

#include "Matrix.h"
#include "../../Tools/VectorTools.h"

namespace Objects {
    Span Matrix::gram_schmidt() const {
        std::vector<Vector> unit_vectors;

        for (size_t col = 0; col < columns(); col++) {
            Vector v = get_column(col);

            for (size_t row = 0; row < col; row++) {
                Vector &u = unit_vectors.at(row);
                v -= Tools::VectorTools::project(u, get_column(col));
            }

            auto norm = v.norm();
            v /= norm;
            if (v.norm() < norm * 1e-12) continue;
            unit_vectors.push_back(v);
        }

        return {unit_vectors};
    }

    QR Matrix::qr_gram_schmidt() const {
        MatrixRaw Q(gram_schmidt());
        MatrixRaw R(rows(), columns());

        for (size_t row = 0; row < rows(); row++) {
            for (size_t col = row; col < columns(); col++) {
                R[row, col] = Tools::VectorTools::inner_product(Q.row(row), get_column(col));
            }
        }

        return {Q, R};
    }
}
