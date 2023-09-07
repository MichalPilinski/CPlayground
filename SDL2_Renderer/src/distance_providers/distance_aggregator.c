#include <float.h>
#include "distance_aggregator.h"

#define MAX_SPHERES 10

Sphere spheres[10];
int sphereCount = 0;

double getSpheresDistance(struct Point3D *point)
{
    double minDistance = DBL_MAX;
    for(int i = 0; i < sphereCount; i++)
    {
        minDistance = fmin(minDistance, getSphereDistance(&spheres[i], point));
    }

    return minDistance;
}

double getSceneDistance(struct Point3D *point) {
    double sphereDistance = getSpheresDistance(point);

    return sphereDistance;
}

void addSphere(struct Point3D *point, double radius)
{
    Sphere sphere = (Sphere) {
            .center = *point,
            .radius = radius
    };

    spheres[sphereCount] = sphere;
    sphereCount++;
}