//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_VECTOR_H
#define JLINEALGCPP_VECTOR_H

#include <cmath>
#include <sstream>

#include "../Matrix/Matrix.h"

//  ________________
// {| a1 | a2 | a3 |
//  | a4 | a5 | a6 |
//  | a7 | a8 | a9 |}
//  ----------------

namespace Objects {
    class DoubleProxy {
        double &element_;
        bool &changed_;
        bool &dim_changed_;
        bool &isNullPtr_;

    public:
        friend class VectorProxy;
        DoubleProxy(double &element, bool &changed, bool &isNullPtr)
            : element_(element), changed_(changed), dim_changed_(changed), isNullPtr_(isNullPtr)
        {}
        DoubleProxy(double &element, bool &changed, bool &dim_changed, bool &isNullPtr)
            : element_(element), changed_(changed), dim_changed_(dim_changed), isNullPtr_(isNullPtr)
        {}

        operator double(); // NOLINT
        DoubleProxy& operator=(double val);
        DoubleProxy& operator=(const DoubleProxy &other);
        DoubleProxy& operator+=(double val);
    };

    class Vector {
        double *elements_ = nullptr;
        int n_ = 0;

        double norm_ = 0;

        bool changed_ = true;
        bool isNullPtr = true;

    public:
        friend class VectorProxy;
        /**
         * Default constructor.\n
         * Initializes vector as a null vector.
         */
        Vector();
        /**
         * Constructs vector from initializer list.\n
         * If newly constructed vector is found to have a norm of 0: resets to null vector.
         */
        Vector(std::initializer_list<double> elements);
        Vector(const Vector &other);
        explicit Vector(int n);
        ~Vector();

        Vector& operator=(const Vector &other);
        DoubleProxy operator[](int p);
        double operator[](int p) const;
        Vector& operator+=(const Vector &other);
        Vector& operator-=(const Vector &other);
        bool operator==(Vector &other);
        bool operator!=(Vector &other);

        double norm();

        [[nodiscard]] int size() const;
        [[nodiscard]] bool is_null_vec() const;
    };

    inline Vector operator+(Vector l, const Vector &r) {
        return l += r;
    }

    inline Vector operator-(Vector l, const Vector &r) {
        return l -= r;
    }
} // Objects

#endif //JLINEALGCPP_VECTOR_H