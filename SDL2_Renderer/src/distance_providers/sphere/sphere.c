#include "sphere.h"

double getSphereDistance(struct Sphere *sphere, struct Point3D *point)
{
    double distance = sqrt(
            pow(sphere->center.x - point->x, 2) +
            pow(sphere->center.y - point->y, 2) +
            pow(sphere->center.z - point->z, 2)
        );

    distance -= sphere->radius;

    return distance;
}