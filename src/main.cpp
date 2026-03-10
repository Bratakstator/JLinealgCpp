#include <iostream>

#include "logging/Logger.h"
#include "objects/Matrix.h"
#include "objects/Vector.h"

using Objects::Vector;
using Objects::Matrix;

int main() {
    Logging::setup();

    Vector v(4);
    Matrix A(4, 4);

    A[0][0] = 5;
    A[0][1] = 3;
    A[0][2] = 6;
    A[0][3] = 4;

    A[1][0] = 2;
    A[1][1] = 6;
    A[1][2] = 9;
    A[1][3] = 2;

    A[2][0] = 2;
    A[2][1] = 7;
    A[2][2] = 8;
    A[2][3] = 1;

    A[3][0] = 7;
    A[3][1] = 5;
    A[3][2] = 6;
    A[3][3] = 4;

    A.print();

    auto [fst, snd] = A.LU();
    fst.print();
    snd.print();

    return 0;
}
