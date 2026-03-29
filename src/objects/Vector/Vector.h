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

    template<typename T>
    struct CacheInstance {
        mutable T is;
        mutable bool valid = false;
    };

    struct VectorCache {
        mutable CacheInstance<dim_t> n{0, false};
        mutable CacheInstance<norm_t> norm{0, false};
    };

    /**
     * @brief Used as a proxy for the vector components.
     *
     * This allows the tracking of when vector components are transformed outside the vector's own methods.
     */
    class ComponentProxy {
        component_t &component_;
        VectorCache &cache_;

    public:
        ComponentProxy(component_t &component, VectorCache &cache) : component_(component), cache_(cache) {}

        operator component_t() const; // NOLINT
        ComponentProxy& operator=(component_t component);
        ComponentProxy& operator=(const ComponentProxy &component);
        ComponentProxy& operator+=(component_t component);
        ComponentProxy& operator-=(component_t component);
        ComponentProxy& operator/=(component_t component);
    };

    /**
     * @brief A class holding components (values) in a (usually) fixed-sized container.
     *
     * The default constructor initializes the vector as a purely logical entity (subject to change/be removed).
     * As a logical entity, the vector is no longer bound by the size of the container. The array will be initialized
     * as an array of size 1, where the operator[] will return 0, or a proxy of 0, regardless of index. This makes it
     * compatible with any other vector regardless of size.
     *
     * operator+= and operator-= will turn the vector from a logical entity, to an actual container object. Here the
     * vector will take the form of the other vector, but only if the other vector is the right operand.
     *
     * When the vector is an actual container object, the only way of resizing it is by assigning it another vector.\n
     * As a container object it will act like a mathematical vector, rather than purely a container of elements.
     */
    class Vector {
        mutable component_t *components_ = nullptr;
        mutable VectorCache cache_;

    public:
        friend class VectorProxy;
        /**
         * Default constructor.\n
         * Initializes vector as a null vector.
         */
        Vector();
        /**
         * Constructs vector from initializer list.
         */
        Vector(std::initializer_list<component_t> components);
        /**
         * Constructs a vector from another vector.
         */
        Vector(const Vector &other);
        /**
         * Creates an empty vector of fixed size.
         */
        explicit Vector(dim_t n);
        ~Vector();

        /**
         * Turns the vector into a copy of another.
         */
        Vector& operator=(const Vector &other);
        ComponentProxy operator[](size_t i);
        component_t operator[](size_t i) const;
        Vector& operator+=(const Vector &other);
        Vector& operator-=(const Vector &other);
        bool operator==(Vector &other);
        bool operator!=(Vector &other);

        /**
         * Returns the norm (length) of the vector.
         */
        norm_t norm() const;

        /**
         * @brief Returns the dimension the vector belongs to.\n
         * As an example, a vector with 3 components is an element of R<sup>3</sup> space.
         */
        [[nodiscard]] dim_t dimension() const;
        /**
         * Returns true if the vector is a null vector.
         */
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