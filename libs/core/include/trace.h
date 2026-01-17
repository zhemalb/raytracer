#pragma once

#include "scene.h"
#include "geometry.h"
#include "ray.h"
#include "vector.h"
#include "hit.h"
#include "trace_context.h"

#include <optional>
#include <limits>

struct TraceDetails {
    Vector position;
    Vector normal;
    double distance;
    const Object* object = nullptr;
    const SphereObject* sphere = nullptr;
};

inline std::optional<TraceDetails> TraceRayDetailed(const Ray& ray, const Scene& scene,
                                                    const Vector& origin) {
    std::optional<Intersection> best;
    double min_dist = std::numeric_limits<double>::infinity();
    const Object* best_obj = nullptr;
    const SphereObject* best_sphere = nullptr;

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
            best_sphere = nullptr;
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
            best_sphere = &sph;
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

        if (DotProduct(nrm, ray.GetDirection()) > 0.0) {
            nrm = Vector{-nrm[0], -nrm[1], -nrm[2]};
        }
    }

    return TraceDetails{pos, nrm, min_dist, best_obj, best_sphere};
}

inline std::optional<Hit> TraceRay(const Ray& ray, const Scene& scene, const Vector& origin) {
    auto details = TraceRayDetailed(ray, scene, origin);
    if (!details) {
        return std::nullopt;
    }
    return Hit{details->position, details->normal, details->distance};
}

inline std::optional<TraceDetails> TraceRayDetailed(const Ray& ray, const TraceContext& ctx,
                                                    const Vector& origin) {
    const Scene& scene = ctx.scene;

    auto tri_hit = IntersectBvhTriangles(ray, scene.GetObjects(), ctx.bvh, origin);
    std::optional<Intersection> best = tri_hit.intersection;
    double min_dist = tri_hit.distance;
    const Object* best_obj = tri_hit.object;
    const SphereObject* best_sphere = nullptr;

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
            best_sphere = &sph;
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

        if (DotProduct(nrm, ray.GetDirection()) > 0.0) {
            nrm = Vector{-nrm[0], -nrm[1], -nrm[2]};
        }
    }

    return TraceDetails{pos, nrm, min_dist, best_obj, best_sphere};
}

inline std::optional<Hit> TraceRay(const Ray& ray, const TraceContext& ctx, const Vector& origin) {
    auto details = TraceRayDetailed(ray, ctx, origin);
    if (!details) {
        return std::nullopt;
    }
    return Hit{details->position, details->normal, details->distance};
}
