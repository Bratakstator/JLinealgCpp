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
        bool &dim_changed_;

    public:
        VectorProxy(Vector &vector, bool &dim_changed)
            : vector_(vector), dim_changed_(dim_changed)
        {}

        operator Vector(); // NOLINT
        VectorProxy& operator=(const Vector &other);
        VectorProxy& operator=(const VectorProxy &other);
        DoubleProxy operator[](int p);
        double operator[](int p) const;
    };

    class Span {
        Vector *space_ = nullptr;
        int n_ = 0;
        int dim_ = 0;
        bool dim_changed = true;

        int any_row_with_non_zero_in_col(int r_, int c_);
        void set_dim();

    public:
        Span();
        explicit Span(std::initializer_list<Vector> space);
        explicit Span(const Span &other);
        explicit Span(int m, int n);
        ~Span();

        Span& operator=(const Span &other);
        VectorProxy operator[](int p);
        Vector operator[](int p) const;

        Span& swap(int p1, int p2);

        [[nodiscard]] int dim() const;
        [[nodiscard]] int room() const;
        [[nodiscard]] int size() const;

        [[nodiscard]] Vector* begin() const;
        [[nodiscard]] Vector* end() const;
        [[nodiscard]] Vector* begin();
        [[nodiscard]] Vector* end();
    };
} // Objects

#endif //JLINEALGCPP_SPAN_H