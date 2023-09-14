#include <float.h>
#include "distance_aggregator.h"
#include "string.h"

#define MAX_SPHERES 10

Sphere spheres[10];
int sphereCount = 0;

void clearSpheres() {
    memset(spheres, 0, sizeof(spheres));
    sphereCount = 0;
}

double comparatorFunction(double sceneDistance, double distance) {
    // Default comparator
    //return fmin(sceneDistance, distance);

    // Smoothing operator
    double k = 8;
    double h = fmax(k - fabs(sceneDistance - distance), 0) / k;
    return fmin(sceneDistance, distance) - (pow(h, 3) * k / 6.0);
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

// Return closest sphere with blended colors based on reverse squared distance
SphereTuple getClosestSphere(struct Point3D *point) {
    struct Sphere minSphere;
    struct Material sphereMaterial = (struct Material) {0, 0, 0, 0, 0,0, 0};

    double minDistance = DBL_MAX;
    double weightSum = 0;
    for(int i = 0; i < sphereCount; i++)
    {
        double sphereDistance = getSphereDistance(&spheres[i], point);
        if(sphereDistance < minDistance) {
            minDistance = sphereDistance;
            minSphere = spheres[i];
        }

        double weight = 1 / sphereDistance;
        weightSum += weight;

        sphereMaterial.color.r += (int)(spheres[i].material.color.r * weight);
        sphereMaterial.color.g += (int)(spheres[i].material.color.g * weight);
        sphereMaterial.color.b += (int)(spheres[i].material.color.b * weight);
    }

    sphereMaterial.ambientConstant = minSphere.material.ambientConstant;
    sphereMaterial.diffuseConstant = minSphere.material.diffuseConstant;
    sphereMaterial.specularConstant = minSphere.material.specularConstant;
    sphereMaterial.shininessConstant = minSphere.material.shininessConstant;

    sphereMaterial.color.r = (int)(sphereMaterial.color.r / weightSum);
    sphereMaterial.color.g = (int)(sphereMaterial.color.g / weightSum);
    sphereMaterial.color.b = (int)(sphereMaterial.color.b / weightSum);

    if(sphereMaterial.color.r > 255) sphereMaterial.color.r = 255;
    if(sphereMaterial.color.g > 255) sphereMaterial.color.g = 255;
    if(sphereMaterial.color.b > 255) sphereMaterial.color.b = 255;

    return (SphereTuple) {
        .sphere = (struct Sphere) {
            .material = sphereMaterial,
            .radius = minSphere.radius,
            .center = minSphere.center
        },
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