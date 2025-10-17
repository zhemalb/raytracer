#pragma once

#include "vector.h"

#include <exception>

class Intersection {
public:
    Intersection([[maybe_unused]] const Vector& position, [[maybe_unused]] const Vector& normal, [[maybe_unused]] double distance) {
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

    const Vector& GetPosition() const {
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

    const Vector& GetNormal() const {
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

    double GetDistance() const {
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
