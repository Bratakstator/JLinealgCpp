//
// Created by Joakim on 22.03.2026.
//

#include "Matrix.h"
#include "../Span/Span.h"
#include "../Vector/Vector.h"

namespace Objects {
    int Matrix::get_non_zero_row_in_col(int col, int current_row) {
        for (int row = current_row; row < rows(); row++) {
            if ((*this)[row, col] != 0) return (*this)[row, col];
        }
        return -1;
    }

    Matrix Matrix::row_echelon() {
        Matrix ech(span_);

        for (int row = 0; row < ech.rows(); row++) {
            int start_col = row;
            if (ech[row, start_col] == 0) {
                int swaprow = row;
                for (int col = start_col; col < ech.columns(); col++) {
                    swaprow = ech.get_non_zero_row_in_col(col, row);
                    if (swaprow != -1) {
                        ech.span_.swap(row, swaprow);
                        break;
                    }
                }
                if (swaprow == -1) return ech;
            }
        }

        return ech;
    }
}
