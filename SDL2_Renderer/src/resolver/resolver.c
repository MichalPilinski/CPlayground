#include <printf.h>
#import "resolver.h"
#include "../distance_providers/distance_aggregator.h"
#include "../light_sources/light_aggregator.h"

#define EPSILON 10E-4
#define HORIZON 10E+3
#define VISIBILITY_SPHERE 1000

struct Vector3D getNormal(struct Point3D *point, double distanceAtPoint) {
    double delta = 10e-4;

    struct Point3D xPoint = (struct Point3D) { .x = point->x + delta, .y = point->y, .z = point->z };
    double xDistance = getSceneDistance(&xPoint);

    struct Point3D yPoint = (struct Point3D) { .x = point->x, .y = point->y + delta, .z = point->z };
    double yDistance = getSceneDistance(&yPoint);

    struct Point3D zPoint = (struct Point3D) { .x = point->x, .y = point->y, .z = point->z + delta };
    double zDistance = getSceneDistance(&zPoint);

    return (struct Vector3D) {
            .x = (xDistance - distanceAtPoint) / delta,
            .y = (yDistance - distanceAtPoint) / delta,
            .z = (zDistance - distanceAtPoint) / delta
    };
}

struct Vector3D calculateAmbientLighting(struct Material *material) {
    struct Vector3D ambientLighting = {1, 1, 1};
    scaleVector3D(&ambientLighting, material->ambientConstant);

    return ambientLighting;
}

struct Vector3D calculateDiffuseLighting(struct Point3D *surfacePoint, struct Material *material, double distance)
{
    // diffuse lighting
    struct Vector3D diffuseLighting = {1, 1, 1};
    struct Vector3D normal = getNormal(surfacePoint, distance);

    double intensity = calculateDiffuseIntensity(surfacePoint, &normal);

    scaleVector3D(&diffuseLighting, material->diffuseConstant);
    scaleVector3D(&diffuseLighting, intensity);

    clampVector3D(&diffuseLighting);

    return diffuseLighting;
}

struct Vector3D calculateSpecularLighting(struct Point3D *surfacePoint, struct Point3D *viewerPoint, struct Material *material, double distance)
{
    struct Vector3D specularLighting = {1, 1, 1 };
    struct Vector3D normal = getNormal(surfacePoint, distance);
    struct Vector3D viewerVector = getVector3D(surfacePoint, viewerPoint);

    double intensity = calculateSpecularIntensity(surfacePoint, &normal, &viewerVector);
    if(intensity < 0) intensity = 0;
    intensity = pow(intensity, material->shininessConstant);

    scaleVector3D(&specularLighting, material->specularConstant);
    scaleVector3D(&specularLighting, intensity);

    clampVector3D(&specularLighting);

    return specularLighting;
}

struct RgbColor calculateLighting(struct Point3D *viewerPoint, struct Point3D *surfacePoint, struct Material *material, double distance) {
    // we are going to represent color by vector for ease of maths
    struct Vector3D color = {material->color.r, material->color.g, material->color.b};
    struct Vector3D light = {0, 0, 0};

    // ambient lighting
    struct Vector3D ambientLighting = calculateAmbientLighting(material);
    addToVector3D(&light, &ambientLighting);

    // diffuse lighting
    struct Vector3D diffuseLighting = calculateDiffuseLighting(surfacePoint, material, distance);
    addToVector3D(&light, &diffuseLighting);

    // specular lighting
    struct Vector3D specularLighting = calculateSpecularLighting(surfacePoint, viewerPoint, material, distance);
    addToVector3D(&light, &specularLighting);

    clampVector3D(&light);

    return (struct RgbColor) {
        .r = (int)(color.x * light.x),
        .g = (int)(color.y * light.y),
        .b = (int)(color.z * light.z)
    };
}

struct RgbColor simulateRay(struct Point3D *start, struct Vector3D *direction)
{
    struct Point3D currentPosition = *start;

    struct Vector3D normalizedDirection = *direction;
    normalizeVector3D(&normalizedDirection);

    double distance = getSceneDistance(&currentPosition);
    double distanceToStart = getPointsDistance(&currentPosition, start);

    int safetyCounter = 150;
    while(distance > EPSILON && distance < HORIZON && distanceToStart < VISIBILITY_SPHERE)
    {
        struct Vector3D nextStep = getScaledVector(&normalizedDirection, distance);
        addVectorToPoint(&currentPosition, &nextStep);

        distance = getSceneDistance(&currentPosition);

        safetyCounter--;
        if(safetyCounter < 0) {
            // Should return yes (its very close to object!)
            break;
        }

        distanceToStart = getPointsDistance(&currentPosition, start);
    }

    if(distance > EPSILON) {
        return (struct RgbColor) {0, 0, 0};
    }

    struct Material material = getMaterialAtPoint(&currentPosition);
    return calculateLighting(start, &currentPosition, &material, distance);
}
