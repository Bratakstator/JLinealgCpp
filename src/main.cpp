#include "logging/Logger.h"
#include "objects/Matrix.h"
#include "objects/Vector.h"

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

    return 0;
}
