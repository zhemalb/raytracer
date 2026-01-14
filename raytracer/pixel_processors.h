#pragma once

#include "camera.h"
#include "image.h"
#include "trace.h"
#include "color.h"
#include "illumination.h"
#include "trace_context.h"

#include <vector>
#include <algorithm>

template <typename T>
void ForEachPixel(int width, int height, T func) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            func(y, x);
        }
    }
}

struct NormalModePixelProcessor {
    int width;
    int height;
    const CameraBasis& basis;
    double aspect;
    double scale;
    const Vector& origin;
    const TraceContext& ctx;
    Image& image;

    void operator()(int y, int x) {
        Vector dir = GenerateRayDirection(x, y, width, height, basis, aspect, scale);
        Ray ray(origin, dir);
        std::optional<Hit> closest_hit = TraceRay(ray, ctx, origin);
        RGB pixel_color = {0, 0, 0};
        if (closest_hit) {
            Vector n = closest_hit->normal;
            n.Normalize();
            if (DotProduct(n, dir) > 0.0) {
                double neg_x = -n[0];
                double neg_y = -n[1];
                double neg_z = -n[2];
                n = Vector{neg_x, neg_y, neg_z};
            }
            pixel_color.r = ToByte(kNormalMapScale * n[0] + kNormalMapOffset);
            pixel_color.g = ToByte(kNormalMapScale * n[1] + kNormalMapOffset);
            pixel_color.b = ToByte(kNormalMapScale * n[2] + kNormalMapOffset);
        }
        image.SetPixel(pixel_color, y, x);
    }
};

struct DepthCollector {
    int width;
    int height;
    const CameraBasis& basis;
    double aspect;
    double scale;
    const Vector& origin;
    const TraceContext& ctx;
    std::vector<double>& depths;
    double& max_depth;

    void operator()(int y, int x) {
        Vector dir = GenerateRayDirection(x, y, width, height, basis, aspect, scale);
        Ray ray(origin, dir);
        std::optional<Hit> closest_hit = TraceRay(ray, ctx, origin);
        if (closest_hit) {
            depths[y * width + x] = closest_hit->distance;
            max_depth = std::max(max_depth, closest_hit->distance);
        }
    }
};

struct DepthRenderer {
    int width;
    const std::vector<double>& depths;
    double max_depth;
    Image& image;

    void operator()(int y, int x) {
        double d = depths[y * width + x];
        RGB pixel_color;
        if (d < 0.0) {
            pixel_color = {kWhiteColor, kWhiteColor, kWhiteColor};
        } else {
            int val = ToByte((max_depth > 0.0) ? (d / max_depth) : 0.0);
            pixel_color = {val, val, val};
        }
        image.SetPixel(pixel_color, y, x);
    }
};

struct FullModePixelProcessor {
    int width;
    int height;
    const CameraBasis& basis;
    double aspect;
    double scale;
    const Vector& origin;
    const TraceContext& ctx;
    int max_depth;
    std::vector<Vector>& hdr_colors;

    void operator()(int y, int x) {
        Vector dir = GenerateRayDirection(x, y, width, height, basis, aspect, scale);
        Ray ray(origin, dir);
        Vector color = ComputeIllumination(ray, ctx, origin, 0, max_depth, false);
        hdr_colors[y * width + x] = color;
    }
};
