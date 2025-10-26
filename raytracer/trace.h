#pragma once

#include "scene.h"
#include "geometry.h"
#include "ray.h"
#include "vector.h"
#include "hit.h"

#include <optional>
#include <limits>

template <typename Range, typename MemberPtr>
void ConsiderAll(const Ray& ray, const Range& range, MemberPtr member_ptr, const Vector& origin,
                 double& min_dist, std::optional<Intersection>& best) {
    for (const typename Range::value_type& item : range) {
        std::optional<Intersection> hit = GetIntersection(ray, item.*member_ptr);
        if (!hit) {
            continue;
        }
        const Vector& pos = hit->GetPosition();
        double dx = pos[0] - origin[0];
        double dy = pos[1] - origin[1];
        double dz = pos[2] - origin[2];
        Vector diff{dx, dy, dz};
        double dist = Length(diff);
        if (dist < min_dist) {
            min_dist = dist;
            best = hit;
        }
    }
}

inline std::optional<Hit> TraceRay(const Ray& ray, const Scene& scene, const Vector& origin) {
    std::optional<Intersection> best;
    double min_dist = std::numeric_limits<double>::infinity();
    const Object* best_obj = nullptr;
    for (const Object& obj : scene.GetObjects()) {
        auto hit = GetIntersection(ray, obj.polygon);
        if (!hit) {
            continue;
        }
        const Vector& pos = hit->GetPosition();
        Vector diff{pos[0] - origin[0], pos[1] - origin[1], pos[2] - origin[2]};
        const double dist = Length(diff);
        if (dist < min_dist) {
            min_dist = dist;
            best = hit;
            best_obj = &obj;
            (void)0;
        }
    }
    for (const SphereObject& sph : scene.GetSphereObjects()) {
        auto hit = GetIntersection(ray, sph.sphere);
        if (!hit) {
            continue;
        }
        const Vector& pos = hit->GetPosition();
        Vector diff{pos[0] - origin[0], pos[1] - origin[1], pos[2] - origin[2]};
        const double dist = Length(diff);
        if (dist < min_dist) {
            min_dist = dist;
            best = hit;
            best_obj = nullptr;
            (void)0;
        }
    }
    if (!best) {
        return std::nullopt;
    }
    Vector pos = best->GetPosition();
    Vector nrm = best->GetNormal();
    if (best_obj && best_obj->normals[0] && best_obj->normals[1] && best_obj->normals[2]) {
        Vector bc = GetBarycentricCoords(best_obj->polygon, pos);
        const Vector& n0 = *best_obj->normals[0];
        const Vector& n1 = *best_obj->normals[1];
        const Vector& n2 = *best_obj->normals[2];
        nrm = Vector(bc[0] * n0[0] + bc[1] * n1[0] + bc[2] * n2[0],
                     bc[0] * n0[1] + bc[1] * n1[1] + bc[2] * n2[1],
                     bc[0] * n0[2] + bc[1] * n1[2] + bc[2] * n2[2]);
        nrm.Normalize();
    }
    return Hit{pos, nrm, min_dist};
}
