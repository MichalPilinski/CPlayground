#include <printf.h>
#import "resolver.h"
#include "../distance_providers/distance_aggregator.h"

#define EPSILON 10E-4
#define HORIZON 10E+3

struct RaySimulationResult simulateRay(struct Point3D *start, struct Vector3D *direction)
{
    struct Point3D currentPosition = *start;

    struct Vector3D normalizedDirection = *direction;
    normalizeVector3D(&normalizedDirection);

    double distance = getSceneDistance(&currentPosition);
    int safetyCounter = 1000;
    while(distance > EPSILON && distance < HORIZON)
    {
        struct Vector3D nextStep = getScaledVector(&normalizedDirection, distance);
        addVectorToPoint(&currentPosition, &nextStep);

        distance = getSceneDistance(&currentPosition);

        safetyCounter--;
        if(safetyCounter < 0) { printf("The forbidden one has been assembled!"); break; }
    }

    int intersectionFlag = distance <= EPSILON;

    return (struct RaySimulationResult) {
        .doesIntersect = intersectionFlag
    };
}