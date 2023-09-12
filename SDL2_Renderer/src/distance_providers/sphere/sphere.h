#ifndef SDL2_RENDERER_SPHERE_H
#define SDL2_RENDERER_SPHERE_H

#include "../../common/geometry.h"
#include "../../common/material.h"

typedef struct Sphere Sphere;

struct Sphere {
    struct Point3D center;
    double radius;
    struct Material material;
};

double getSphereDistance(struct Sphere *sphere, struct Point3D *point);

#endif //SDL2_RENDERER_SPHERE_H
