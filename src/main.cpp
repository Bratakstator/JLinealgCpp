#include <vector>

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

    // Matrix B = A * A;

    A.print();
    std::cout << "\n";
    A.row_echelon().print();
    std::cout << "\n";
    A.reduced_row_echelon().print();
    std::cout << "\n";

    std::cout << "det(A) = " <<  A.determinant() << "\n";

    std::vector<int> a = {1, 2, 3};

    a.size()

    return 0;
}
