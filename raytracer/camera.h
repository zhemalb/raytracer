#pragma once

#include "vector.h"

#include <cmath>

struct CameraBasis {
    Vector forward;
    Vector right;
    Vector up;
};

inline CameraBasis BuildCameraBasis(const Vector& look_from, const Vector& look_to) {
    double x = look_to[0] - look_from[0];
    double y = look_to[1] - look_from[1];
    double z = look_to[2] - look_from[2];
    Vector forward{x, y, z};
    forward.Normalize();
    Vector world_up{0.0, 1.0, 0.0};
    if (std::fabs(forward[0]) < 1e-6 && std::fabs(forward[2]) < 1e-6) {
        if (forward[1] > 0.0) {
            world_up = Vector{0.0, 0.0, 1.0};
        } else {
            world_up = Vector{0.0, 0.0, -1.0};
        }
    }
    Vector right = CrossProduct(forward, world_up);
    right.Normalize();
    Vector up = CrossProduct(right, forward);
    up.Normalize();
    return CameraBasis{forward, right, up};
}

inline Vector GenerateRayDirection(int x, int y, int width, int height, const CameraBasis& basis,
                                   double aspect, double scale) {
    const double nx = (x + 0.5) / static_cast<double>(width);
    const double ny = (y + 0.5) / static_cast<double>(height);
    const double cx = (2.0 * nx - 1.0) * aspect * scale;
    const double cy = (1.0 - 2.0 * ny) * scale;
    const double dx = cx * basis.right[0] + cy * basis.up[0] + basis.forward[0];
    const double dy = cx * basis.right[1] + cy * basis.up[1] + basis.forward[1];
    const double dz = cx * basis.right[2] + cy * basis.up[2] + basis.forward[2];
    Vector dir{dx, dy, dz};
    dir.Normalize();
    return dir;
}
