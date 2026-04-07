//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_MATRIX_H
#define JLINEALGCPP_MATRIX_H

#include "../Span/Span.h"

namespace Objects {
    using det_t = component_t;
    using code_t = int;

    /**
     * Struct containing an index / value and return code of a method.\n
     * This is with the assumption of the method using this struct is capable of self-correcting if code indicates
     * failure.
     */
    struct Code {
        size_t index;
        code_t code;
    };

    struct EigenPairs {
        mutable Vector values;
        mutable Span vectors;
    };

    struct PLU {
        mutable Span P;
        mutable Span L;
        mutable Span U;
    };

    struct QR {
        mutable Span Q;
        mutable Span R;
    };

    struct MatrixCache {
        mutable CacheInstance<bool> identity;
        mutable CacheInstance<bool> diagonal;
        mutable CacheInstance<bool> symmetrical;
        mutable CacheInstance<bool> skew_symmetrical;

        mutable CacheInstance<bool> invertible;

        mutable CacheInstance<bool> upper_triangular;
        mutable CacheInstance<bool> lower_triangular;

        mutable CacheInstance<bool> orthonormal_columns;
        mutable CacheInstance<bool> orthogonal;

        mutable CacheInstance<det_t> det;

        mutable CacheInstance<Span> REF;
        mutable CacheInstance<bool> REF_force_eq_one;
        mutable CacheInstance<Span> RREF;
        mutable CacheInstance<PLU> plu;
        mutable CacheInstance<QR> qr;
        mutable CacheInstance<Span> inverse;
        mutable CacheInstance<EigenPairs> eigen;

        void invalidate() { // NOLINT
            identity.valid = false;
            diagonal.valid = false;
            symmetrical.valid = false;
            skew_symmetrical.valid = false;

            invertible.valid = false;

            upper_triangular.valid = false;
            lower_triangular.valid = false;

            orthonormal_columns.valid = false;
            orthogonal.valid = false;

            det.valid = false;

            REF = {{}, false};
            REF_force_eq_one.valid = false;
            RREF = {{}, false};
            plu = {{}, false};
            qr = {{}, false};
            inverse = {{}, false};
            eigen = {{}, false};
        }

        void invertible_true() { // NOLINT
            identity.is = identity.valid = true;
            diagonal.is = identity.valid = true;
            symmetrical.is = identity.valid = true;

            invertible.is = invertible.valid = true;

            upper_triangular.is = upper_triangular.valid = true;
            lower_triangular.is = lower_triangular.valid = true;

            orthonormal_columns.is = orthonormal_columns.valid = true;
            orthogonal.is = orthogonal.valid = true;

            det.is = 1;
            det.valid = true;
        }
        void invertible_false() { // NOLINT
            identity.is = identity.valid = false;
            diagonal.is = identity.valid = false;
            symmetrical.is = identity.valid = false;

            invertible.is = invertible.valid = false;

            upper_triangular.is = upper_triangular.valid = false;
            lower_triangular.is = lower_triangular.valid = false;

            orthonormal_columns.is = orthonormal_columns.valid = false;
            orthogonal.is = orthogonal.valid = false;
        }
    };

    class ComponentPPProxy {
        component_t &proxy_;
        MatrixCache &mCache_;
        SpanCache &sCache_;
        VectorCache & vCache_;

    public:
        ComponentPPProxy(component_t &proxy, MatrixCache &mCache, SpanCache &sCache, VectorCache &vCache)
            : proxy_(proxy), mCache_(mCache), sCache_(sCache), vCache_(vCache) {}

        ComponentPPProxy(ComponentPProxy &comppproxy, MatrixCache &mCache) // NOLINT
            : proxy_(comppproxy.proxy_), mCache_(mCache), sCache_(comppproxy.sCache_), vCache_(comppproxy.vCache_) {}

        operator component_t() const; // NOLINT
        ComponentPPProxy& operator=(component_t component);
        ComponentPPProxy& operator=(const ComponentPPProxy &component);
        ComponentPPProxy& operator+=(component_t component);
        ComponentPPProxy& operator-=(component_t component);
        ComponentPPProxy& operator/=(component_t component);
    };


    template<typename T>
    concept VectorProxyOnly = std::same_as<T, VectorProxy> || std::same_as<T, VectorProxy&>;

    template<VectorProxyOnly T>
        class AugmentedMatrixVectorPair {
        T v1_;
        T v2_;

        friend AugmentedMatrixVectorPair<VectorProxy> operator*
            (const component_t &component, AugmentedMatrixVectorPair<VectorProxy> v);

    public:
        AugmentedMatrixVectorPair(T v1, T v2) : v1_(v1), v2_(v2) {}

        operator Vector() const;

        AugmentedMatrixVectorPair operator-=(AugmentedMatrixVectorPair v);
        AugmentedMatrixVectorPair operator/=(const component_t component); // NOLINT
        AugmentedMatrixVectorPair operator*=(component_t component);
    };

    class AugmentedMatrix {
        Matrix &A_;
        Matrix &B_;

    public:
        AugmentedMatrix(Matrix &A, Matrix &B) : A_(A), B_(B) {}

        [[nodiscard]] Matrix& A() const;
        [[nodiscard]] Matrix& B() const;

        AugmentedMatrixVectorPair<VectorProxy&> operator[](size_t i);
        AugmentedMatrixVectorPair<VectorProxy> operator[](size_t i) const;

        code_t swap_rows(const size_t r1, const size_t r2, const code_t code=0) const; // NOLINT
    };

    inline AugmentedMatrixVectorPair<VectorProxy> operator*
        (const component_t &component, AugmentedMatrixVectorPair<VectorProxy> v) {
        return {v.v1_ * component, v.v2_ * component};
    }

    /**
     * @brief Matrix is a fixed-sized container operating as a mathematical object.
     *
     * Does not contain default constructor, must be made either with a list of vectors or with an explicit size.
     */
    class Matrix {
        mutable Span row_space_;
        mutable MatrixCache cache_;

        /**
         * @brief Swaps rows in the matrix.
         *
         * returns code, the codes are:\n
         * 0: successful swap.\n
         * 1: whether this indicates failure is context-specific. This code indicates that the indexes for r1 and r2
         * are the same, i.e. they point to the same row.\n
         * O: if the code passed as an argument is a non-zero value, the method returns 128 + code, this so it can't
         * be confused with its own codes, the actual code can then be retrieved with:\n
         * <code> code_t code = swap_rows(2, 4, 15); // returns 128 + 15 = 143 </code>\n
         * <code> code_t code_actual = code - 128; // code_actual = 143 - 128 = 15 </code>
         */
        code_t swap_rows(size_t r1, size_t r2, code_t code=0);

        static void do_row_echelon(const AugmentedMatrix &AB, bool pivots_must_be_one=false);
        static void do_reduced_row_echelon(const AugmentedMatrix &AB);
    public:
        Matrix() = delete;

        /**
         * Creates an nxn identity matrix.
         */
        explicit Matrix(size_t n);
        /**
         * Creates an all-zero mxn matrix.
         */
        Matrix(size_t m, size_t n);
        /**
         * Creates a new matrix from the span of another row space.
         */
        explicit Matrix(const Span &row_space);
        /**
         * Creates a matrix from a list of vectors, using initializer list.
         */
        explicit Matrix(std::initializer_list<Vector> row_space);
        /**
         * Creates a copy of another matrix.
         */
        Matrix(const Matrix &other);

        ComponentPPProxy operator[](size_t m, size_t n); // NOLINT
        component_t operator[](size_t m, size_t n) const;
        Matrix operator*(const Matrix &other);

        [[nodiscard]] Vector get_column(size_t n) const;
        [[nodiscard]] Vector get_row(size_t m) const;

        /**
         * @brief Gets the first instance of a non-zero row-element in given column, starting at current_row.
         */
        Code get_non_zero_row_in_col(size_t col, size_t current_row) const;
        /**
         * @brief Gets the first non-zero column-element in a given row.
         */
        Code get_first_non_zero_col(size_t row) const;

        /**
         * @brief Reduces a copy of the matrix to row echelon form.
         *
         * The reduced copy will be stored in this matrix's echelons struct in echelons_.REF and returned.\n
         * The parameter 'pivots_must_be_one' tells the method whether it should turn all pivot-elements to 1 (true),
         * or only turn elements below pivots to 0.
         */
        Matrix row_echelon(bool pivots_must_be_one=false) const;
        /**
         * @brief Reduces a copy of the matrix to reduced row echelon form.
         *
         * The reduced copy will be stored in this matrix's echelons struct in echelons_.RREF and returned.
         */
        Matrix reduced_row_echelon() const;

        /**
         * @brief Returns the inverted matrix.
         * Also stores the inverted matrix in cache.
         */
        Matrix invert() const;

        /**
         * @brief Returns the transposed matrix.
         */
        Matrix transpose() const;

        /**
         * @brief Returns the determinant of the matrix.
         */
        [[nodiscard]] det_t determinant() const; // NOLINT

        /**
         * @brief Returns whether this matrix is a diagonal matrix.
         */
        [[nodiscard]] bool diagonal() const;
        /**
         * Returns whether this matrix is an identity matrix.
         */
        [[nodiscard]] bool identity() const;

        [[nodiscard]] size_t rows() const;
        [[nodiscard]] size_t columns() const;
        [[nodiscard]] size_t rank() const;

        void print() const;

        friend class AugmentedMatrix;
    };
} // Objects

#endif //JLINEALGCPP_MATRIX_H