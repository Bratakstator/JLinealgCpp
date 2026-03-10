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

    std::cout << "After making v and A" << "\n";

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

    std::cout << "After inserting into A" << "\n";

    A.print();

    std::cout << "After printing A" << "\n";

    auto [fst, snd] = A.LU();
    std::cout << "After making LU" << "\n";
    fst.print();
    std::cout << "After printing L" << "\n";
    snd.print();
    std::cout << "After printing U" << "\n";

    return 0;
}
