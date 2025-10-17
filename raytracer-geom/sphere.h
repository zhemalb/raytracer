#pragma once

#include "vector.h"

#include <exception>

class Sphere {
public:
    Sphere([[maybe_unused]] const Vector& center, [[maybe_unused]] double radius) {
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

    const Vector& GetCenter() const {
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

    double GetRadius() const {
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
};
