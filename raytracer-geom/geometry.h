#pragma once

#include "vector.h"
#include "sphere.h"
#include "intersection.h"
#include "triangle.h"
#include "ray.h"

#include <optional>

std::optional<Intersection> GetIntersection(const Ray& ray, const Sphere& sphere) {
    const Vector& center = sphere.GetCenter();
    const Vector& origin = ray.GetOrigin();
    const Vector& direction = ray.GetDirection();

    double oc_x = origin[0] - center[0];
    double oc_y = origin[1] - center[1];
    double oc_z = origin[2] - center[2];
    const Vector oc = {oc_x, oc_y, oc_z};
    double a = DotProduct(direction, direction);
    double b = 2.0 * DotProduct(oc, direction);
    double c = DotProduct(oc, oc) - sphere.GetRadius() * sphere.GetRadius();
    double d = b * b - 4 * a * c;
    if (d < 0) {
        return std::nullopt;
    }
    
    double sqrt_d = std::sqrt(d);
    double t = (-b - sqrt_d) / (2.0 * a);
    if (t < 0) {
        t = (-b + sqrt_d) / (2.0 * a);
        if (t < 0) {
            return std::nullopt;
        }
    }
    
    double pos_x = origin[0] + direction[0] * t;
    double pos_y = origin[1] + direction[1] * t;
    double pos_z = origin[2] + direction[2] * t;
    const Vector position = {pos_x, pos_y, pos_z};
    double norm_x = position[0] - center[0];
    double norm_y = position[1] - center[1];
    double norm_z = position[2] - center[2];
    Vector normal = {norm_x, norm_y, norm_z};
    normal.Normalize();

    if (DotProduct(normal, direction) > 0.0) {
        double neg_x = -normal[0];
        double neg_y = -normal[1];
        double neg_z = -normal[2];
        normal = Vector{neg_x, neg_y, neg_z};
    }

    double diff_x = position[0] - origin[0];
    double diff_y = position[1] - origin[1];
    double diff_z = position[2] - origin[2];
    const Vector diff = {diff_x, diff_y, diff_z};
    double dist = Length(diff);
    return Intersection(position, normal, dist);
}

std::optional<Intersection> GetIntersection(const Ray& ray, const Triangle& triangle) {
    const Vector& a = triangle[0];
    const Vector& b = triangle[1];
    const Vector& c = triangle[2];

    double e1_x = b[0] - a[0];
    double e1_y = b[1] - a[1];
    double e1_z = b[2] - a[2];
    Vector e1{e1_x, e1_y, e1_z};
    double e2_x = c[0] - a[0];
    double e2_y = c[1] - a[1];
    double e2_z = c[2] - a[2];
    Vector e2{e2_x, e2_y, e2_z};
    const Vector& direction = ray.GetDirection();
    const Vector& origin = ray.GetOrigin();
    Vector pvec = CrossProduct(direction, e2);
    double det = DotProduct(e1, pvec);
    if (det < 1e-8 && det > -1e-8) {
        return std::nullopt;
    }

    double inv = 1.0 / det;
    Vector tvec{origin[0] - a[0], origin[1] - a[1], origin[2] - a[2]};
    double u = DotProduct(tvec, pvec) * inv;
    if (u < 0 || u > 1) {
        return std::nullopt;
    }

    Vector qvec = CrossProduct(tvec, e1);
    double v = DotProduct(direction, qvec) * inv;
    if (v < 0 || u + v > 1) {
        return std::nullopt;
    }

    double t = DotProduct(e2, qvec) * inv;
    if (t < 0) {
        return std::nullopt;
    }

    double pos_x = origin[0] + direction[0] * t;
    double pos_y = origin[1] + direction[1] * t;
    double pos_z = origin[2] + direction[2] * t;
    Vector position{pos_x, pos_y, pos_z};
    Vector normal = CrossProduct(e1, e2);
    normal.Normalize();
    if (DotProduct(normal, direction) > 0.0) {
        double neg_x = -normal[0];
        double neg_y = -normal[1];
        double neg_z = -normal[2];
        normal = Vector{neg_x, neg_y, neg_z};
    }

    double diff_x = position[0] - origin[0];
    double diff_y = position[1] - origin[1];
    double diff_z = position[2] - origin[2];
    Vector diff{diff_x, diff_y, diff_z};
    const double dist = Length(diff);
    return Intersection(position, normal, dist);
}

Vector Reflect(const Vector& ray, const Vector& normal) {
    Vector n = normal;
    n.Normalize();

    double nray = DotProduct(n, ray);
    double x = ray[0] - 2.0 * nray * n[0];
    double y = ray[1] - 2.0 * nray * n[1];
    double z = ray[2] - 2.0 * nray * n[2];

    return Vector(x, y, z);
}

std::optional<Vector> Refract(const Vector& ray, const Vector& normal, double eta) {
    Vector r = ray;
    Vector n = normal;
    r.Normalize();
    n.Normalize();

    double nray = DotProduct(n, r);
    double k = 1.0 - eta * eta * (1.0 - nray * nray);

    if (k < 0.0) {
        return std::nullopt;
    }

    double c = eta * nray + std::sqrt(k);
    double x = eta * ray[0] - c * normal[0];
    double y = eta * ray[1] - c * normal[1];
    double z = eta * ray[2] - c * normal[2];

    return Vector(x, y, z);
}

Vector GetBarycentricCoords(const Triangle& triangle, const Vector& point) {
    const Vector& a = triangle[0];
    const Vector& b = triangle[1];
    const Vector& c = triangle[2];

    double v0x = b[0] - a[0];
    double v0y = b[1] - a[1];
    double v0z = b[2] - a[2];
    double v1x = c[0] - a[0];
    double v1y = c[1] - a[1];
    double v1z = c[2] - a[2];
    double v2x = point[0] - a[0];
    double v2y = point[1] - a[1];
    double v2z = point[2] - a[2];
    double d00 = v0x * v0x + v0y * v0y + v0z * v0z;
    double d01 = v0x * v1x + v0y * v1y + v0z * v1z;
    double d11 = v1x * v1x + v1y * v1y + v1z * v1z;
    double d20 = v2x * v0x + v2y * v0y + v2z * v0z;
    double d21 = v2x * v1x + v2y * v1y + v2z * v1z;
    double d = d00 * d11 - d01 * d01;
    double v = (d11 * d20 - d01 * d21) / d;
    double w = (d00 * d21 - d01 * d20) / d;
    double u = 1.0 - v - w;
    return Vector(u, v, w);
}
