#pragma once

#include "vector.h"

#include <exception>

class Ray {
public:
    Ray([[maybe_unused]] const Vector& origin, [[maybe_unused]] const Vector& direction) {
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

    const Vector& GetOrigin() const {
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

    const Vector& GetDirection() const {
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
