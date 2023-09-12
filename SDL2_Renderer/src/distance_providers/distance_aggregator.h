#ifndef SDL2_RENDERER_DISTANCE_AGGREGATOR_H
#define SDL2_RENDERER_DISTANCE_AGGREGATOR_H


#include "sphere/sphere.h"
typedef struct {
    struct Sphere sphere;
    double distance;
} SphereTuple;

double getSpheresDistance(struct Point3D *point);
double getSceneDistance(struct Point3D *point);
void addSphere(struct Point3D *point, double radius, struct Material material);
struct Material getMaterialAtPoint(struct Point3D *point);

#endif //SDL2_RENDERER_DISTANCE_AGGREGATOR_H
