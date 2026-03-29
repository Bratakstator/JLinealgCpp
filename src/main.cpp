#include "logging/Logger.h"
#include "objects/Matrix/Matrix.h"
#include "objects/Vector/Vector.h"

using Objects::Vector;
using Objects::Matrix;

int main() {
    // Fix how proxies function
    Logging::setup();

    Matrix A(
        {
            {3, 3, 3},
            {4, 5, 2},
            {2, 6, 3}
        }
    );
    Matrix B(
        {
            {4, 5, 2},
            {3, 3, 3},
            {2, 6, 3}
        }
    );

    std::cout << "Matrix A:\n";
    A.print();
    std::cout << "A's row echelon form:\n";
    A.row_echelon().print();
    std::cout << "det(A) = " <<  A.determinant() << "\n\n";

    std::cout << "Matrix B:\n";
    B.print();
    std::cout << "B's row echelon form:\n";
    B.row_echelon().print();
    std::cout << "det(B) = " << B.determinant() << "\n";

    return 0;
}
