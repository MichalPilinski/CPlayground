#include "sphere.h"

double getSphereDistance(struct Sphere *sphere, struct Point3D *point)
{
    double distance = sqrt(
            pow(sphere->center.x - fmod(point->x + 3, 6) - 3, 2) +
            pow(sphere->center.y - fmod(point->y + 3, 6) - 3, 2) +
            pow(sphere->center.z - point->z, 2)
        );

    distance -= sphere->radius;

    return distance;
}