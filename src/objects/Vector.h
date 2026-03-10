//
// Created by Joakim on 3/9/2026.
//

#ifndef JLINEALGCPP_VECTOR_H
#define JLINEALGCPP_VECTOR_H

namespace Objects {
    class Vector {
        double *data_;
        int n_;

    public:
        Vector() {
            n_ = 1;
            data_ = new double[1];
        };
        explicit Vector(const int n) : n_(n) {
            data_ = new double[n];
            for (int i = 0; i < n; i++) {
                data_[i] = 0;
            }
        }
        explicit Vector(Vector &other) {
            n_ = other.n_;
            data_ = new double[n_];
            for (int i = 0; i < n_; i++) {
                data_[i] = other[i];
            }
        }
        Vector &operator=(const Vector &other) noexcept {
            n_ = other.n_;

            if (other.data_ == nullptr) return *this;

            data_ = new double[n_];
            for (int i = 0; i < n_; i++) {
                data_[i] = other[i];
            }

            return *this;
        }
        ~Vector() {
            delete data_;
        }


        double& operator[](const int n) const { //NOLINT
            return data_[n];
        }
        double& operator[](const int n) { //NOLINT
            return data_[n];
        }
    };
} // Objects

#endif //JLINEALGCPP_VECTOR_H