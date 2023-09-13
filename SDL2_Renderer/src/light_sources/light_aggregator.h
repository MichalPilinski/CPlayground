#ifndef SDL2_RENDERER_LIGHT_AGGREGATOR_H
#define SDL2_RENDERER_LIGHT_AGGREGATOR_H

#include "../common/geometry.h"

struct Light {
    struct Point3D position;
    double intensity;
};

void addLight(struct Point3D *position, double intensity);
double calculateDiffuseIntensity(struct Point3D *surfacePoint, struct Vector3D *normal);
double calculateSpecularIntensity(struct Point3D *surfacePoint, struct Vector3D *normal, struct Vector3D *viewerVector);

#endif //SDL2_RENDERER_LIGHT_AGGREGATOR_H
