//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_MATRIX_H
#define JLINEALGCPP_MATRIX_H

#include <iostream>
#include <iomanip>

#include "../Span/Span.h"

namespace Objects {
    using det_t = component_t;
    using code_t = int;


    struct MatrixRaw {
        size_t m;
        size_t n;
        component_t **elements_;

        MatrixRaw() {
            m = 1;
            n = 1;

            elements_ = new component_t*[m];
            for (size_t row = 0; row < m; row++) elements_[row] = new component_t[n];
            zero_out();
        }

        MatrixRaw(const size_t rows, const size_t columns) {
            m = rows;
            n = columns;

            elements_ = new component_t*[m];
            for (size_t row = 0; row < m; row++) elements_[row] = new component_t[n];
            zero_out();
        }

        MatrixRaw(const Span &S, const bool transpose=false) {
            if (transpose) {
                m = S.size();
                n = S.vector_dimension();
            }
            else {
                m = S.vector_dimension();
                n = S.size();
            }

            elements_ = new component_t*[m];
            for (int row = 0; row < m; row++) elements_[row] = new component_t[n];

            for (size_t row = 0; row < m; row++) {
                for (size_t col = 0; col < n; col++) {
                    if (transpose) elements_[row][col] = S[row][col];
                    else elements_[row][col] = S[col][row];
                }
            }
        }

        ~MatrixRaw() {
            for (size_t i = 0; i < m; i++) delete[] elements_[i];
            delete[] elements_;
        }


        component_t& operator[](const size_t row, const size_t column) { return elements_[row][column]; } // NOLINT
        component_t operator[](const size_t row, const size_t column) const { return elements_[row][column]; }

        [[nodiscard]] Vector row(const size_t i) const {
            Vector v(n);
            for (size_t col = 0; col < n; col++) v[col] = elements_[i][col];
            return v;
        }
        [[nodiscard]] Vector column(const size_t i) const {
            Vector v(m);
            for (size_t row = 0; row < m; row++) v[row] = elements_[row][i];
            return v;
        }

    private:
        void zero_out() { // NOLINT
            for (size_t row = 0; row < m; row++) {
                for (size_t col = 0; col < n; col++) {
                    elements_[row][col] = 0;
                }
            }
        }
    };

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
        mutable MatrixRaw Q;
        mutable MatrixRaw R;
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
            : proxy_(proxy), mCache_(mCache), sCache_(sCache), vCache_(vCache)
        {}

        ComponentPPProxy(ComponentPProxy &comppproxy, MatrixCache &mCache) // NOLINT
            : proxy_(comppproxy.proxy_), mCache_(mCache), sCache_(comppproxy.sCache_), vCache_(comppproxy.vCache_)
        {}

        operator component_t() const; // NOLINT
        ComponentPPProxy& operator=(component_t component);
        ComponentPPProxy& operator=(const ComponentPPProxy &component);
        ComponentPPProxy& operator+=(component_t component);
        ComponentPPProxy& operator-=(component_t component);
        ComponentPPProxy& operator*=(component_t component);
        ComponentPPProxy& operator/=(component_t component);
    };


    template<typename T>
    concept VectorProxyOnly = std::same_as<T, VectorProxy> || std::same_as<T, Vector>;

    template<VectorProxyOnly T>
    class AugmentedMatrixVectorPair {
        T v1_;
        T v2_;

        MatrixCache &v1_cache_;
        MatrixCache &v2_cache_;

        friend AugmentedMatrixVectorPair<Vector> operator*
            (const component_t &component, AugmentedMatrixVectorPair<Vector> v);
        friend AugmentedMatrixVectorPair<Vector> operator*
            (const component_t &component, AugmentedMatrixVectorPair<VectorProxy> v);

        friend class AugmentedMatrixVectorPair<VectorProxy>;
        friend class AugmentedMatrixVectorPair<Vector>;

        //friend AugmentedMatrixVectorPair<VectorProxy> operator-=
        //    (AugmentedMatrixVectorPair<VectorProxy> &vp, AugmentedMatrixVectorPair<Vector> v);

    public:
        AugmentedMatrixVectorPair(VectorProxy v1, VectorProxy v2, MatrixCache &v1_cache, MatrixCache &v2_cache)
            : v1_(v1), v2_(v2), v1_cache_(v1_cache), v2_cache_(v2_cache)
        {}

        AugmentedMatrixVectorPair(const Vector& v1, const Vector& v2, MatrixCache &v1_cache, MatrixCache &v2_cache)
            : v1_cache_(v1_cache), v2_cache_(v2_cache) {
            v1_ = v1;
            v2_ = v2;
        }

        operator Vector() const { return v1_; }
        operator AugmentedMatrixVectorPair<Vector>() const {
            return AugmentedMatrixVectorPair<Vector>(v1_, v2_, v1_cache_, v2_cache_);
        }

        template<VectorProxyOnly K>
        AugmentedMatrixVectorPair& operator-=(const AugmentedMatrixVectorPair<K> &v) {
            v1_ -= v.v1_;
            v2_ -= v.v2_;

            if (std::same_as<T, VectorProxy>) {
                v1_cache_.invalidate();
                v2_cache_.invalidate();
            }

            return *this;
        }
        AugmentedMatrixVectorPair& operator/=(const component_t component) {
            v1_ /= component;
            v2_ /= component;

            if (std::same_as<T, VectorProxy>) {
                v1_cache_.invalidate();
                v2_cache_.invalidate();
            }

            return *this;
        }
        AugmentedMatrixVectorPair& operator*=(const component_t component) {
            v1_ *= component;
            v2_ *= component;

            if (std::same_as<T, VectorProxy>) {
                v1_cache_.invalidate();
                v2_cache_.invalidate();
            }

            return *this;
        }
    };

    inline AugmentedMatrixVectorPair<Vector> operator*
        (const component_t &component, AugmentedMatrixVectorPair<Vector> v) {
        return {v.v1_ *= component, v.v2_ *= component, v.v1_cache_, v.v2_cache_};
    }
    inline AugmentedMatrixVectorPair<Vector> operator*
        (const component_t &component, AugmentedMatrixVectorPair<VectorProxy> v) { // NOLINT
        return component * AugmentedMatrixVectorPair<Vector>(v);
    }

    //inline AugmentedMatrixVectorPair<VectorProxy> operator-=
    //    (AugmentedMatrixVectorPair<VectorProxy> &vp, const AugmentedMatrixVectorPair<Vector> v) {
    //    vp.v1_ -= v.v1_;
    //    vp.v2_ -= v.v2_;
    //    return vp;
    //}

    class AugmentedMatrix;

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

        Matrix& operator+=(const Matrix &B);
        Matrix& operator-=(const Matrix &B);
        Matrix& operator*=(const Matrix &B);

        Matrix& operator*=(component_t c);
        Matrix& operator/=(component_t c);

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


        Span gram_schmidt() const;
        QR qr_gram_schmidt() const;


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
        /**
        * Returns whether this matrix is invertible.
        */
        [[nodiscard]] bool invertible() const;

        [[nodiscard]] size_t rows() const;
        [[nodiscard]] size_t columns() const;
        [[nodiscard]] size_t rank() const;

        void print() const;

        friend class AugmentedMatrix;
    };

    inline Matrix operator+(Matrix A, const Matrix &B) { return A += B; }
    inline Matrix operator-(Matrix A, const Matrix &B) { return A -= B; }
    inline Matrix operator*(Matrix A, const Matrix &B) { return A *= B; }

    inline Vector operator*(const Matrix &A, const Vector &v) {
        if (A.columns() != v.dimension()) throw std::invalid_argument("Matrix and vector of incompatible sizes.");

        Vector w(v.dimension());
        for (size_t row = 0; row < A.rows(); row++) {
            for (size_t col = 0; col < A.columns(); col++) {
                w[row] += A[row, col] * v[col];
            }
        }
        return w;
    }
    inline Vector operator*(const Vector &v, const Matrix &A) {
        return A.transpose() * v;
    }

    inline Matrix operator*(Matrix A, const component_t &c) { return A *= c; }
    inline Matrix operator*(const component_t &c, Matrix A) { return A *= c; }
    inline Matrix operator/(Matrix A, const component_t &c) { return A /= c; }

    class AugmentedMatrix {
        Matrix &A_;
        Matrix &B_;

    public:
        AugmentedMatrix(Matrix &A, Matrix &B) : A_(A), B_(B) {}

        [[nodiscard]] Matrix& A() const { return A_; }
        [[nodiscard]] Matrix& B() const { return B_; }

        AugmentedMatrixVectorPair<VectorProxy> operator[](const size_t i) const {
            return {A_.row_space_[i], B_.row_space_[i], A_.cache_, B_.cache_};
        }
        //AugmentedMatrixVectorPair<Vector> operator[](const size_t i) const {
        //    return {A_.row_space_[i], B_.row_space_[i]};
        //}

        code_t swap_rows(const size_t r1, const size_t r2, const code_t code) const { // NOLINT
            B_.swap_rows(r1, r2, code);
            return A_.swap_rows(r1, r2, code);
        }

        void print() const {
            std::cout << std::fixed << std::setprecision(2);

            for (size_t row = 0; row < A_.rows(); row++) {
                for (size_t col = 0; col < A_.columns(); col++) {
                    std::cout << A_[row, col] << "  ";
                }
                std::cout << "\t\t| ";
                for (size_t col = 0; col < B_.columns(); col++) {
                    std::cout << B_[row, col] << "  ";
                }
                std::cout << "\n";
            }
        }
    };
} // Objects

#endif //JLINEALGCPP_MATRIX_H