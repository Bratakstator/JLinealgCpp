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

    class Span {
        Vector *space_ = nullptr;
        size_t n_ = 0;
        dim_t rank_ = 0;
        bool changed_ = true;

        int any_row_with_non_zero_in_col(int r_, int c_);
        void set_dim();

    public:
        Span();
        explicit Span(std::initializer_list<Vector> space);
        explicit Span(const Span &other);
        explicit Span(size_t m, size_t n);
        ~Span();

        Span& operator=(const Span &other);
        VectorProxy operator[](size_t i);
        Vector operator[](size_t i) const;

        Span& swap(size_t p1, size_t p2);

        [[nodiscard]] dim_t rank() const;
        [[nodiscard]] dim_t vector_dimension() const;
        [[nodiscard]] size_t size() const;

        [[nodiscard]] Vector* begin() const;
        [[nodiscard]] Vector* end() const;
        [[nodiscard]] Vector* begin();
        [[nodiscard]] Vector* end();
    };
} // Objects

#endif //JLINEALGCPP_SPAN_H