//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_VECTOR_H
#define JLINEALGCPP_VECTOR_H

#include <sstream>

//  ________________
// {| a1 | a2 | a3 |
//  | a4 | a5 | a6 |
//  | a7 | a8 | a9 |}
//  ----------------

namespace Objects {
    using component_t = double;
    using norm_t = double;
    using size_t = std::initializer_list<component_t>::size_type;
    using dim_t = size_t;

    class ComponentProxy {
        component_t &component_;
        bool &vec_changed_;
        bool &span_changed_;
        bool &isNullPtr_;

    public:
        friend class VectorProxy;
        ComponentProxy(component_t &component, bool &changed, bool &isNullPtr)
            : component_(component), vec_changed_(changed), span_changed_(changed), isNullPtr_(isNullPtr)
        {}
        ComponentProxy(component_t &component, bool &vec_changed, bool &span_changed, bool &isNullPtr)
            : component_(component), vec_changed_(vec_changed), span_changed_(span_changed), isNullPtr_(isNullPtr)
        {}

        operator component_t(); // NOLINT
        ComponentProxy& operator=(component_t component);
        ComponentProxy& operator=(const ComponentProxy &component);
        ComponentProxy& operator+=(component_t component);
        ComponentProxy& operator-=(component_t component);
        ComponentProxy& operator/=(component_t component);
    };

    class Vector {
        component_t *components_ = nullptr;
        dim_t n_ = 0;

        norm_t norm_ = 0;

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
        Vector(std::initializer_list<component_t> components);
        Vector(const Vector &other);
        explicit Vector(dim_t n);
        ~Vector();

        Vector& operator=(const Vector &other);
        ComponentProxy operator[](size_t i);
        component_t operator[](size_t i) const;
        Vector& operator+=(const Vector &other);
        Vector& operator-=(const Vector &other);
        bool operator==(Vector &other);
        bool operator!=(Vector &other);

        norm_t norm();

        [[nodiscard]] dim_t dimension() const;
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