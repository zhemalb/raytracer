#pragma once

#include "aabb.h"
#include "geometry.h"
#include "object.h"

#include <algorithm>
#include <limits>
#include <numeric>
#include <optional>
#include <vector>

struct BvhNode {
    Aabb box;
    int left = -1;
    int right = -1;
    int start = 0;
    int count = 0;
    bool is_leaf = false;
};

struct Bvh {
    std::vector<BvhNode> nodes;
    std::vector<int> indices;
};

inline Aabb GetTriangleAabb(const Triangle& tri) {
    const Vector& a = tri[0];
    const Vector& b = tri[1];
    const Vector& c = tri[2];
    Vector min_v = ComponentwiseMin(ComponentwiseMin(a, b), c);
    Vector max_v = ComponentwiseMax(ComponentwiseMax(a, b), c);
    return Aabb{min_v, max_v};
}

namespace bvh_detail {

inline double GetAxisValue(const Vector& v, int axis) {
    return v[static_cast<size_t>(axis)];
}

inline int BuildNode(Bvh& bvh, const std::vector<Aabb>& aabbs, const std::vector<Vector>& centroids,
                     int start, int end, int leaf_size) {
    const int node_index = static_cast<int>(bvh.nodes.size());
    bvh.nodes.push_back(BvhNode{});

    Aabb node_box = aabbs[bvh.indices[start]];
    for (int i = start + 1; i < end; ++i) {
        node_box = Union(node_box, aabbs[bvh.indices[i]]);
    }

    const int count = end - start;
    if (count <= leaf_size) {
        auto& node = bvh.nodes[node_index];
        node.box = node_box;
        node.start = start;
        node.count = count;
        node.is_leaf = true;
        return node_index;
    }

    Aabb centroid_box{centroids[bvh.indices[start]], centroids[bvh.indices[start]]};
    for (int i = start + 1; i < end; ++i) {
        const Vector& c = centroids[bvh.indices[i]];
        centroid_box.min = ComponentwiseMin(centroid_box.min, c);
        centroid_box.max = ComponentwiseMax(centroid_box.max, c);
    }

    const double ext_x = centroid_box.max[0] - centroid_box.min[0];
    const double ext_y = centroid_box.max[1] - centroid_box.min[1];
    const double ext_z = centroid_box.max[2] - centroid_box.min[2];

    int axis = 0;
    double best_ext = ext_x;
    if (ext_y > best_ext) {
        axis = 1;
        best_ext = ext_y;
    }
    if (ext_z > best_ext) {
        axis = 2;
        best_ext = ext_z;
    }

    if (best_ext < 1e-12) {
        auto& node = bvh.nodes[node_index];
        node.box = node_box;
        node.start = start;
        node.count = count;
        node.is_leaf = true;
        return node_index;
    }

    const int mid = start + count / 2;
    std::nth_element(bvh.indices.begin() + start, bvh.indices.begin() + mid,
                     bvh.indices.begin() + end, [&](int lhs, int rhs) {
                         return GetAxisValue(centroids[lhs], axis) <
                                GetAxisValue(centroids[rhs], axis);
                     });

    const int left = BuildNode(bvh, aabbs, centroids, start, mid, leaf_size);
    const int right = BuildNode(bvh, aabbs, centroids, mid, end, leaf_size);

    auto& node = bvh.nodes[node_index];
    node.box = node_box;
    node.left = left;
    node.right = right;
    node.is_leaf = false;
    return node_index;
}

}  // namespace bvh_detail

inline Bvh BuildBvh(const std::vector<Object>& objects) {
    Bvh bvh;
    const int n = static_cast<int>(objects.size());
    if (n == 0) {
        return bvh;
    }

    std::vector<Aabb> aabbs;
    std::vector<Vector> centroids;
    aabbs.reserve(objects.size());
    centroids.reserve(objects.size());
    for (const Object& obj : objects) {
        Aabb box = GetTriangleAabb(obj.polygon);
        aabbs.push_back(box);
        centroids.push_back(Centroid(box));
    }

    bvh.indices.resize(objects.size());
    std::iota(bvh.indices.begin(), bvh.indices.end(), 0);
    bvh.nodes.reserve(objects.size() * 2);

    constexpr int kLeafSize = 8;
    (void)bvh_detail::BuildNode(bvh, aabbs, centroids, 0, n, kLeafSize);

    return bvh;
}

struct BvhTriangleHit {
    std::optional<Intersection> intersection;
    const Object* object = nullptr;
    double distance = std::numeric_limits<double>::infinity();
};

inline BvhTriangleHit IntersectBvhTriangles(const Ray& ray, const std::vector<Object>& objects,
                                            const Bvh& bvh, const Vector& origin) {
    BvhTriangleHit best;
    if (bvh.nodes.empty()) {
        return best;
    }

    const double dir_len = Length(ray.GetDirection());
    if (dir_len <= 1e-30) {
        return best;
    }
    const double inv_dir_len = 1.0 / dir_len;

    std::vector<int> stack;
    stack.reserve(64);
    stack.push_back(0);

    while (!stack.empty()) {
        const int node_index = stack.back();
        stack.pop_back();
        const BvhNode& node = bvh.nodes[node_index];

        const double t_max = std::isfinite(best.distance) ? (best.distance * inv_dir_len)
                                                          : std::numeric_limits<double>::infinity();
        if (!IntersectAabb(ray, node.box, 0.0, t_max)) {
            continue;
        }

        if (node.is_leaf) {
            for (int i = 0; i < node.count; ++i) {
                const int obj_index = bvh.indices[node.start + i];
                const Object& obj = objects[obj_index];
                auto hit = GetIntersection(ray, obj.polygon);
                if (!hit) {
                    continue;
                }
                const Vector& pos = hit->GetPosition();
                Vector diff{pos[0] - origin[0], pos[1] - origin[1], pos[2] - origin[2]};
                const double dist = Length(diff);
                if (dist < best.distance) {
                    best.distance = dist;
                    best.intersection = hit;
                    best.object = &obj;
                }
            }
            continue;
        }

        if (node.left >= 0) {
            stack.push_back(node.left);
        }
        if (node.right >= 0) {
            stack.push_back(node.right);
        }
    }

    return best;
}
