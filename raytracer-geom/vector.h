#pragma once

#include <array>
#include <cstddef>
#include <cmath>

class Vector;
double Length(const Vector& v);

class Vector {
public:
    Vector() : data_{0.0, 0.0, 0.0} {
    }
    Vector(double x, double y, double z) : data_{x, y, z} {
    }

    double& operator[](size_t ind) {
        return data_[ind];
    }

    double operator[](size_t ind) const {
        return data_[ind];
    }

    void Normalize() {
        const double len = Length(*this);
        if (len <= 1e-30) {
            return;
        }
        const double mult = 1.0 / len;
        data_[0] *= mult;
        data_[1] *= mult;
        data_[2] *= mult;
    }

private:
    std::array<double, 3> data_;
};

inline Vector operator+(const Vector& a, const Vector& b) {
    return Vector{a[0] + b[0], a[1] + b[1], a[2] + b[2]};
}

inline Vector operator-(const Vector& a, const Vector& b) {
    return Vector{a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}

inline Vector operator-(const Vector& v) {
    return Vector{-v[0], -v[1], -v[2]};
}

inline Vector operator*(const Vector& v, double s) {
    return Vector{v[0] * s, v[1] * s, v[2] * s};
}

inline Vector operator*(double s, const Vector& v) {
    return v * s;
}

inline double DotProduct(const Vector& a, const Vector& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline Vector CrossProduct(const Vector& a, const Vector& b) {
    return Vector(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}

inline double Length(const Vector& v) {
    return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

inline Vector MultiplyComponentwise(const Vector& a, const Vector& b) {
    return Vector{a[0] * b[0], a[1] * b[1], a[2] * b[2]};
}
