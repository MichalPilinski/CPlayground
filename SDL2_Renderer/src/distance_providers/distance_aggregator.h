#ifndef SDL2_RENDERER_DISTANCE_AGGREGATOR_H
#define SDL2_RENDERER_DISTANCE_AGGREGATOR_H


#include "sphere/sphere.h"

double getSpheresDistance(struct Point3D *point);
double getSceneDistance(struct Point3D *point);
void addSphere(struct Point3D *point, double radius);

#endif //SDL2_RENDERER_DISTANCE_AGGREGATOR_H
