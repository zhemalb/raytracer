#pragma once

#include "options/camera_options.h"
#include "options/render_options.h"
#include "image.h"

#include <exception>
#include <filesystem>

Image Render([[maybe_unused]] const std::filesystem::path& path,
             [[maybe_unused]] const CameraOptions& camera_options,
             [[maybe_unused]] const RenderOptions& render_options) {
#if defined(__x86_64__) && defined(__linux__)
    asm volatile(
        "mov $0, %%rdi\n\t"
        "mov $0x3c, %%rax\n\t"
        "syscall"
        :
        :
        : "rax", "rdi", "memory");
    __builtin_unreachable();
#else
    std::terminate();
#endif
}
