#pragma once

#include <algorithm>

constexpr double kToByte = 255.0;
constexpr double kPixelCenter = 0.5;
constexpr double kNormalMapScale = 0.5;
constexpr double kNormalMapOffset = 0.5;
constexpr int kWhiteColor = 255;

inline int ToByte(double x) {
    return static_cast<int>(std::clamp(x, 0.0, 1.0) * kToByte + kPixelCenter);
}
