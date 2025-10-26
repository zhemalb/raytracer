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
    ConsiderAll(ray, scene.GetObjects(), &Object::polygon, origin, min_dist, best);
    ConsiderAll(ray, scene.GetSphereObjects(), &SphereObject::sphere, origin, min_dist, best);
    if (!best) {
        return std::nullopt;
    }
    return Hit{best->GetPosition(), best->GetNormal(), min_dist};
}
