//
// Created by Joakim on 3/10/2026.
//

#ifndef JLINEALGCPP_SPAN_H
#define JLINEALGCPP_SPAN_H

#include <iostream>

#include "../Vector/Vector.h"

//  ________________
// {| a1 | a2 | a3 |
//  | a4 | a5 | a6 |
//  | a7 | a8 | a9 |}
//  ----------------

namespace Objects {
    /**
     * @brief Acts as a proxy for the vector.
     *
     * This allows the tracking of changes to the vector outside the Span's own methods.
     */
    class VectorProxy {
        Vector &vector_;
        bool &changed_;

    public:
        VectorProxy(Vector &vector, bool &changed)
            : vector_(vector), changed_(changed)
        {}

        operator Vector(); // NOLINT
        VectorProxy& operator=(const Vector &other);
        VectorProxy& operator=(const VectorProxy &other);
        ComponentProxy operator[](size_t i);
        component_t operator[](size_t i) const;
    };

    /**
     * @brief Is a container of vectors representing the span of a vector space.
     */
    class Span {
        Vector *space_ = nullptr;
        size_t n_ = 0;
        dim_t rank_ = 0;
        bool changed_ = true;

        int any_row_with_non_zero_in_col(int r_, int c_);
        void set_dim();

    public:
        /**
         * Creates a span of size 0 containing only a null vector.
         */
        Span();
        /**
         * Creates a span from initializer list.
         */
        explicit Span(std::initializer_list<Vector> space);
        /**
         * Makes a copy of another span.
         */
        explicit Span(const Span &other);
        /**
         * Creates a span of specified size.
         */
        explicit Span(size_t m, size_t n);
        ~Span();

        Span& operator=(const Span &other);
        VectorProxy operator[](size_t i);
        Vector operator[](size_t i) const;

        /**
         * Swaps the place of two vectors.\n
         * Returns itself.
         */
        Span& swap(size_t p1, size_t p2);

        /**
         * Returns the rank of the span.
         */
        [[nodiscard]] dim_t rank() const;
        /**
         * Returns the dimension the vectorspace belongs to.
         */
        [[nodiscard]] dim_t vector_dimension() const;
        /**
         * Returns the amount of vectors in the span (regardless of dependence).
         */
        [[nodiscard]] size_t size() const;

        [[nodiscard]] Vector* begin() const;
        [[nodiscard]] Vector* end() const;
        [[nodiscard]] Vector* begin();
        [[nodiscard]] Vector* end();
    };
} // Objects

#endif //JLINEALGCPP_SPAN_H