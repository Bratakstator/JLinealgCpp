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

        operator double() { // NOLINT
            return element_;
        }

        DoubleProxy& operator=(const double val) {
            if (isNullPtr_) return *this;

            element_ = val;
            changed_ = true;
            dim_changed_ = true;
            return *this;
        }
        DoubleProxy& operator=(const DoubleProxy &other) {
            if (isNullPtr_) return *this;

            if (this != &other) {
                element_ = other.element_;
                changed_ = true;
                dim_changed_ = true;
            }
            return *this;
        }

        DoubleProxy& operator+=(const double val) {
            if (isNullPtr_) return *this;
            element_ += val;
            changed_ = true;
            dim_changed_ = true;

            return *this;
        }
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
        Vector() { elements_ = new double[1]; }

        /**
         * Constructs vector from initializer list.\n
         * If newly constructed vector is found to have a norm of 0: resets to null vector.
         */
        Vector(const std::initializer_list<double> elements) {
            n_ = elements.size();
            if (n_ == 0) return;

            elements_ = new double[n_];

            int i = 0;
            for (const auto element : elements) {
                elements_[i] = element;
                i++;
            }

            if (norm() == 0) {
                n_ = 0;
                delete elements_;
                elements_ = new double[1];
                changed_ = true;
            }
            else isNullPtr = false;
        }

        Vector(const Vector &other) {
            n_ = other.n_;
            norm_ = other.norm_;
            changed_ = other.changed_;
            isNullPtr = other.isNullPtr;

            elements_ = new double[n_];
            for (int i = 0; i < n_; i++) {
                elements_[i] = other[i];
            }
        }

        explicit Vector(const int n) {
            n_ = n;
            elements_ = new double[n_];
            isNullPtr = false;

            for (int i = 0; i < n_; i++) {
                elements_[i] = 0;
            }
        }

        ~Vector() {
            delete[] elements_;
            elements_ = nullptr;
        }

        Vector& operator=(const Vector &other) {
            if (this == &other) return *this;
            n_ = other.n_;
            norm_ = other.norm_;
            changed_ = other.changed_;
            isNullPtr = other.isNullPtr;

            elements_ = new double[n_];

            for (int i = 0; i < n_; i++) {
                elements_[i] = other.elements_[i];
            }
            return *this;
        }


        DoubleProxy operator[](int p) { // NOLINT
            if (n_ == 0) return {*elements_, changed_, isNullPtr};
            if (p >= n_) {
                std::cout << "\n";
                std::stringstream err("Index out of range:\n");
                err << "Requested index: " << p << ", but size of Vector is: " << n_ << "\n";
                throw std::out_of_range(err.str());
            }
            return {elements_[p], changed_, isNullPtr};
        }
        double operator[](const int p) const {
            if (n_ == 0) return 0;
            if (p >= n_) {
                std::cout << "\n";
                std::stringstream err("Index out of range:\n");
                err << "Requested index: " << p << ", but size of Vector is: " << n_ << "\n";
                throw std::out_of_range(err.str());
            }
            return elements_[p];
        }

        Vector& operator+=(const Vector &other) {
            if (isNullPtr && !other.is_null_vec()) {
                n_ = other.size();
                delete[] elements_;
                elements_ = new double[n_];

                for (int i = 0; i < n_; i++) elements_[i] = other[i];
                isNullPtr = false;
                changed_ = true;

                return *this;
            }
            if (other.is_null_vec()) return *this;

            for (int i = 0; i < n_; i++) elements_[i] += other[i];
            changed_ = true;

            return *this;
        }

        Vector& operator-=(const Vector &other) {
            if (isNullPtr && !other.is_null_vec()) {
                n_ = other.size();
                delete[] elements_;
                elements_ = new double[n_];

                for (int i = 0; i < n_; i++) elements_[i] = -other[i];
                isNullPtr = false;
                changed_ = true;

                return *this;
            }
            if (other.is_null_vec()) return *this;

            for (int i = 0; i < n_; i++) elements_[i] -= other[i];
            changed_ = true;

            return *this;
        }

        bool operator==(Vector &other) {
            if (!(isNullPtr && other.is_null_vec())) return false;
            if (n_ != other.size()) return false;
            if (norm() != other.norm()) return false;
            for (int i = 0; i < n_; i++) if (elements_[i] != other[i]) return false;
            return true;
        }
        bool operator!=(Vector &other) { return *this == other; }


        double norm() { // NOLINT
            if (!changed_) return norm_;

            double sum = 0;
            for (int i = 0; i < n_; i++) sum += elements_[i] * elements_[i];
            norm_ = std::sqrt(sum);
            changed_ = false;

            return norm_;
        }


        [[nodiscard]] int size() const { return n_; }
        [[nodiscard]] bool is_null_vec() const { return isNullPtr; }
    };

    inline Vector operator+(Vector l, const Vector &r) {
        return l += r;
    }

    inline Vector operator-(Vector l, const Vector &r) {
        return l -= r;
    }
} // Objects

#endif //JLINEALGCPP_VECTOR_H