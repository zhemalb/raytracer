#pragma once

#include "ray.h"
#include "vector.h"

#include <algorithm>
#include <cmath>

struct Aabb {
    Vector min;
    Vector max;
};

inline Vector ComponentwiseMin(const Vector& a, const Vector& b) {
    return Vector{std::min(a[0], b[0]), std::min(a[1], b[1]), std::min(a[2], b[2])};
}

inline Vector ComponentwiseMax(const Vector& a, const Vector& b) {
    return Vector{std::max(a[0], b[0]), std::max(a[1], b[1]), std::max(a[2], b[2])};
}

inline Aabb Union(const Aabb& a, const Aabb& b) {
    return Aabb{ComponentwiseMin(a.min, b.min), ComponentwiseMax(a.max, b.max)};
}

inline Vector Centroid(const Aabb& box) {
    return Vector{0.5 * (box.min[0] + box.max[0]), 0.5 * (box.min[1] + box.max[1]),
                  0.5 * (box.min[2] + box.max[2])};
}

inline bool IntersectAabb(const Ray& ray, const Aabb& box, double t_min, double t_max) {
    const Vector& origin = ray.GetOrigin();
    const Vector& dir = ray.GetDirection();

    for (size_t axis = 0; axis < 3; ++axis) {
        const double d = dir[axis];
        const double o = origin[axis];
        const double lo = box.min[axis];
        const double hi = box.max[axis];

        if (std::fabs(d) < 1e-12) {
            if (o < lo || o > hi) {
                return false;
            }
            continue;
        }

        double t0 = (lo - o) / d;
        double t1 = (hi - o) / d;
        if (t0 > t1) {
            std::swap(t0, t1);
        }

        t_min = std::max(t_min, t0);
        t_max = std::min(t_max, t1);
        if (t_max < t_min) {
            return false;
        }
    }

    return true;
}
