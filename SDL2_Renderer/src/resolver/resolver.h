#ifndef SDL2_RENDERER_RESOLVER_H
#define SDL2_RENDERER_RESOLVER_H

#include "../common/geometry.h"
#include "../common/material.h"

// Will have more properties in the future
struct RaySimulationResult {
    int doesIntersect;
    struct RgbColor color;
};

struct RgbColor simulateRay(struct Point3D *start, struct Vector3D *direction);

#endif //SDL2_RENDERER_RESOLVER_H
