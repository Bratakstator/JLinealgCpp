#include "logging/Logger.h"
#include "objects/Matrix/Matrix.h"
#include "objects/Vector/Vector.h"

using Objects::Vector;
using Objects::Matrix;

int main() {
    Logging::setup();

    Matrix A(
        {{3, 3, 3},
         {4, 5, 2},
         {2, 6, 3}}
    );

    Matrix B = A * A;

    std::cout << B[0, 0] << "\t" << B[0, 1] << "\t" << B[0, 2] << "\n";
    std::cout << B[1, 0] << "\t" << B[1, 1] << "\t" << B[1, 2] << "\n";
    std::cout << B[2, 0] << "\t" << B[2, 1] << "\t" << B[2, 2] << "\n";

    return 0;
}
