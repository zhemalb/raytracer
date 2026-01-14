#pragma once

#include "options/camera_options.h"
#include "options/render_options.h"
#include "image.h"
#include "scene.h"
#include "camera.h"
#include "pixel_processors.h"
#include "postprocessing.h"
#include "trace_context.h"

#include <filesystem>
#include <vector>
#include <cmath>

Image Render(const std::filesystem::path& path, const CameraOptions& camera_options,
             const RenderOptions& render_options) {
    Image image(camera_options.screen_width, camera_options.screen_height);
    Scene scene = ReadScene(path);
    TraceContext ctx{scene};
    const Vector origin = camera_options.look_from;
    const CameraBasis basis = BuildCameraBasis(camera_options.look_from, camera_options.look_to);
    const double aspect = static_cast<double>(camera_options.screen_width) /
                          static_cast<double>(camera_options.screen_height);
    const double scale = std::tan(camera_options.fov / 2.0);
    const int width = camera_options.screen_width;
    const int height = camera_options.screen_height;
    if (render_options.mode == RenderMode::kNormal) {
        ForEachPixel(
            width, height,
            NormalModePixelProcessor{width, height, basis, aspect, scale, origin, ctx, image});
    } else if (render_options.mode == RenderMode::kDepth) {
        std::vector<double> depths(width * height, -1.0);
        double max_depth = 0.0;
        ForEachPixel(
            width, height,
            DepthCollector{width, height, basis, aspect, scale, origin, ctx, depths, max_depth});
        ForEachPixel(width, height, DepthRenderer{width, depths, max_depth, image});
    } else if (render_options.mode == RenderMode::kFull) {
        std::vector<Vector> hdr_colors(width * height, Vector{0, 0, 0});
        ForEachPixel(width, height,
                     FullModePixelProcessor{width, height, basis, aspect, scale, origin, ctx,
                                            render_options.depth, hdr_colors});
        ApplyToneMapping(hdr_colors);
        WriteImageWithGamma(hdr_colors, width, height, image);
    }
    return image;
}
