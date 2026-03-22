#include "logging/Logger.h"
#include "objects/Matrix/Matrix.h"
#include "objects/Vector/Vector.h"

using Objects::Vector;
using Objects::Matrix;

int main() {
    /*
     * Look into using allocators instead of dynamic new / delete
     */

    Logging::setup();

    Matrix A(
        {{3, 3, 3},
         {4, 5, 2},
         {2, 6, 3}}
    );

    Matrix B = A * A;

    std::cout << B[1, 2];

    return 0;
}
