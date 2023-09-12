#include <float.h>
#include "distance_aggregator.h"

#define MAX_SPHERES 10

Sphere spheres[10];
int sphereCount = 0;

double comparatorFunction(double sceneDistance, double distance) {
    return fmin(sceneDistance, distance);
}

double getSpheresDistance(struct Point3D *point)
{
    double minDistance = DBL_MAX;
    for(int i = 0; i < sphereCount; i++)
    {
        minDistance = comparatorFunction(minDistance, getSphereDistance(&spheres[i], point));
    }

    return minDistance;
}

SphereTuple getClosestSphere(struct Point3D *point) {
    struct Sphere minSphere;
    double minDistance = DBL_MAX;
    for(int i = 0; i < sphereCount; i++)
    {
        double sphereDistance = getSphereDistance(&spheres[i], point);
        if(sphereDistance < minDistance) {
            minDistance = sphereDistance;
            minSphere = spheres[i];
        }
    }

    return (SphereTuple) {
        .sphere = minSphere,
        .distance = minDistance
    };
}

double getSceneDistance(struct Point3D *point) {
    double sphereDistance = getSpheresDistance(point);

    return sphereDistance;
}

struct Material getMaterialAtPoint(struct Point3D *point) {
    double minDistance = DBL_MAX;
    struct Material minMaterial;

    SphereTuple sphereTuple = getClosestSphere(point);
    // material should never be uninitialized in this func
    if(sphereTuple.distance < minDistance) {
        minDistance = sphereTuple.distance;
        minMaterial = sphereTuple.sphere.material;
    }

    return minMaterial;
}

void addSphere(struct Point3D *point, double radius, struct Material material)
{
    Sphere sphere = (Sphere) {
            .center = *point,
            .radius = radius,
            .material = material
    };

    spheres[sphereCount] = sphere;
    sphereCount++;
}