#pragma once

#include "vector.h"

#include <cstddef>
#include <array>

class Triangle {
public:
    Triangle(const Vector& a, const Vector& b, const Vector& c) : vectors_{{a, b, c}} {
    }

    const Vector& operator[](size_t ind) const {
        return vectors_[ind];
    }
    double Area() const {
        const Vector& a = vectors_[0];
        const Vector& b = vectors_[1];
        const Vector& c = vectors_[2];
        const Vector ab = {b[0] - a[0], b[1] - a[1], b[2] - a[2]};
        const Vector ac = {c[0] - a[0], c[1] - a[1], c[2] - a[2]};
        return 0.5 * Length(CrossProduct(ab, ac));
    }

private:
    std::array<Vector, 3> vectors_;
};
