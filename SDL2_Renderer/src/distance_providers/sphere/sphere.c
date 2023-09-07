#include "sphere.h"

double getSphereDistance(struct Sphere *sphere, struct Point3D *point)
{
    double distance = getPointsDistance(&sphere->center, point);
    distance -= sphere->radius;

    return distance;
}