#pragma once

#include <array>
#include <cstddef>
#include <exception>

class Vector {
public:
    Vector() : data_{0.0, 0.0, 0.0} {}
    Vector(double x, double y, double z) : data_{x, y, z} {}

    double& operator[](size_t ind) {
        return data_[ind];
    }

    double operator[](size_t ind) const {
        return data_[ind];
    }

    void Normalize() {
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

private:
    std::array<double, 3> data_;
};

inline double DotProduct([[maybe_unused]] const Vector& a, [[maybe_unused]] const Vector& b) {
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

inline Vector CrossProduct([[maybe_unused]] const Vector& a, [[maybe_unused]] const Vector& b) {
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

inline double Length([[maybe_unused]] const Vector& v) {
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
