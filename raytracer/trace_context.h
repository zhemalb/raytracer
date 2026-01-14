#pragma once

#include "bvh.h"
#include "scene.h"

struct TraceContext {
    const Scene& scene;
    Bvh bvh;

    explicit TraceContext(const Scene& s) : scene(s), bvh(BuildBvh(s.GetObjects())) {
    }
};
