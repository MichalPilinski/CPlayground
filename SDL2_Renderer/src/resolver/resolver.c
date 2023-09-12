#include <printf.h>
#import "resolver.h"
#include "../distance_providers/distance_aggregator.h"

#define EPSILON 10E-4
#define HORIZON 10E+3
#define VISIBILITY_SPHERE 100

struct RaySimulationResult simulateRay(struct Point3D *start, struct Vector3D *direction)
{
    struct Point3D currentPosition = *start;

    struct Vector3D normalizedDirection = *direction;
    normalizeVector3D(&normalizedDirection);

    double distance = getSceneDistance(&currentPosition);
    double distanceToStart = getPointsDistance(&currentPosition, start);

    int safetyCounter = 50;
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

    int intersectionFlag = distance <= EPSILON;
    struct Material material;
    if(intersectionFlag) {
        material = getMaterialAtPoint(&currentPosition);
    }

    return (struct RaySimulationResult) {
        .doesIntersect = intersectionFlag,
        .material = material
    };
}