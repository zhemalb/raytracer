#pragma once

#include "options/camera_options.h"
#include "options/render_options.h"
#include "image.h"
#include "scene.h"
#include "camera.h"
#include "pixel_processors.h"

#include <filesystem>
#include <vector>
#include <cmath>

Image Render(const std::filesystem::path& path, const CameraOptions& camera_options,
             const RenderOptions& render_options) {
    Image image(camera_options.screen_width, camera_options.screen_height);
    Scene scene = ReadScene(path);
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
            NormalModePixelProcessor{width, height, basis, aspect, scale, origin, scene, image});
    } else if (render_options.mode == RenderMode::kDepth) {
        std::vector<double> depths(width * height, -1.0);
        double max_depth = 0.0;
        ForEachPixel(
            width, height,
            DepthCollector{width, height, basis, aspect, scale, origin, scene, depths, max_depth});
        ForEachPixel(width, height, DepthRenderer{width, depths, max_depth, image});
    } else if (render_options.mode == RenderMode::kFull) {
        // placeholder
    }
    return image;
}
