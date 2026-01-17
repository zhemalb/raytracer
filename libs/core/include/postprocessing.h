#pragma once

#include "vector.h"
#include "image.h"
#include "color.h"

#include <vector>
#include <algorithm>
#include <cmath>

void ApplyToneMapping(std::vector<Vector>& colors) {
    double max_c = 0.0;
    for (const Vector& c : colors) {
        max_c = std::max(max_c, c[0]);
        max_c = std::max(max_c, c[1]);
        max_c = std::max(max_c, c[2]);
    }

    double cc = max_c * max_c;

    for (Vector& с : colors) {
        for (int i = 0; i < 3; ++i) {
            double v_in = с[i];
            double v_out = (v_in * (1.0 + v_in / cc)) / (1.0 + v_in);
            с[i] = v_out;
        }
    }
}

void WriteImageWithGamma(const std::vector<Vector>& colors, int width, int height, Image& image) {
    const double g_inv = 1.0 / 2.2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Vector& color = colors[y * width + x];

            double r_g = std::pow(std::max(0.0, color[0]), g_inv);
            double g_g = std::pow(std::max(0.0, color[1]), g_inv);
            double b_g = std::pow(std::max(0.0, color[2]), g_inv);

            RGB pixel;
            pixel.r = ToByte(r_g);
            pixel.g = ToByte(g_g);
            pixel.b = ToByte(b_g);

            image.SetPixel(pixel, y, x);
        }
    }
}
