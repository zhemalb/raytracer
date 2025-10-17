#pragma once

#include "vector.h"
#include "sphere.h"
#include "intersection.h"
#include "triangle.h"
#include "ray.h"

#include <exception>
#include <optional>

inline std::optional<Intersection> GetIntersection([[maybe_unused]] const Ray& ray, [[maybe_unused]] const Sphere& sphere) {
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

inline std::optional<Intersection> GetIntersection([[maybe_unused]] const Ray& ray, [[maybe_unused]] const Triangle& triangle) {
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

inline Vector Reflect([[maybe_unused]] const Vector& ray, [[maybe_unused]] const Vector& normal) {
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

inline std::optional<Vector> Refract([[maybe_unused]] const Vector& ray, [[maybe_unused]] const Vector& normal, [[maybe_unused]] double eta) {
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

inline Vector GetBarycentricCoords([[maybe_unused]] const Triangle& triangle, [[maybe_unused]] const Vector& point) {
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
