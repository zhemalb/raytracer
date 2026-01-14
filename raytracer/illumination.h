#pragma once

#include "trace.h"

#include <algorithm>
#include <cmath>

constexpr double kEps = 1e-5;

Vector ComputeIllumination(const Ray& ray, const TraceContext& ctx, const Vector& origin,
                           int current_depth, int max_depth, bool inside_object = false) {
    if (current_depth >= max_depth) {
        return Vector{0, 0, 0};
    }

    auto hit = TraceRayDetailed(ray, ctx, origin);
    if (!hit) {
        return Vector{0, 0, 0};
    }

    const Material* material = nullptr;
    if (hit->object) {
        material = hit->object->material;
    } else if (hit->sphere) {
        material = hit->sphere->material;
    }

    if (!material) {
        return Vector{0, 0, 0};
    }

    const Vector pos = hit->position;
    const Vector norm = hit->normal;
    Vector color = material->ambient_color + material->intensity;

    Vector diffuse_sum{0, 0, 0};
    Vector specular_sum{0, 0, 0};

    Vector to_camera = -ray.GetDirection();
    to_camera.Normalize();

    for (const Light& l : ctx.scene.GetLights()) {
        Vector to_light = l.position - pos;
        double light_distance = Length(to_light);
        to_light.Normalize();

        Vector shadow_origin = pos + norm * kEps;
        Ray shadow_ray(shadow_origin, to_light);
        auto shadow_hit = TraceRayDetailed(shadow_ray, ctx, shadow_origin);

        if (shadow_hit && shadow_hit->distance < light_distance - kEps) {
            continue;
        }

        double diffuse_factor = std::max(0.0, DotProduct(to_light, norm));
        diffuse_sum = diffuse_sum + l.intensity * diffuse_factor;

        Vector reflected_light = Reflect(-to_light, norm);
        double spec_factor = std::max(0.0, DotProduct(reflected_light, to_camera));
        double spec_intensity = std::pow(spec_factor, material->specular_exponent);
        specular_sum = specular_sum + l.intensity * spec_intensity;
    }

    Vector phong_contribution = MultiplyComponentwise(material->diffuse_color, diffuse_sum) +
                                MultiplyComponentwise(material->specular_color, specular_sum);
    color = color + material->albedo[0] * phong_contribution;

    if (material->albedo[1] > kEps && !inside_object) {
        Vector reflected_dir = Reflect(ray.GetDirection(), norm);
        Vector reflected_origin = pos + norm * kEps;
        Ray reflected_ray(reflected_origin, reflected_dir);

        Vector reflected_color = ComputeIllumination(reflected_ray, ctx, reflected_origin,
                                                     current_depth + 1, max_depth, false);

        color = color + material->albedo[1] * reflected_color;
    }

    if (material->albedo[2] > kEps) {
        const double eta =
            inside_object ? material->refraction_index : (1.0 / material->refraction_index);
        if (auto refracted_dir_opt = Refract(ray.GetDirection(), norm, eta)) {
            const Vector& refracted_dir = *refracted_dir_opt;
            Vector refracted_origin = pos - norm * kEps;
            Ray refracted_ray(refracted_origin, refracted_dir);

            double transparency = inside_object ? 1.0 : material->albedo[2];

            Vector refracted_color = ComputeIllumination(
                refracted_ray, ctx, refracted_origin, current_depth + 1, max_depth, !inside_object);

            color = color + transparency * refracted_color;
        }
    }

    return color;
}
