#include "light_aggregator.h"

struct Light lights[10];
int lightsCount = 0;

void addLight(struct Point3D *position, double intensity)
{
    struct Light light = (struct Light) {
        .position = *position,
        .intensity = intensity
    };

    lights[lightsCount] = light;
    lightsCount++;
}

double getIntensity(struct Point3D *position, struct Light *light)
{
    double distance = getPointsDistance(position, &light->position);
    return light->intensity / pow(distance, 2);
}

double calculateDiffuseIntensity(struct Point3D *surfacePoint, struct Vector3D *normal)
{
    double intensity = 0;
    for(int i = 0; i < lightsCount; i++) {
        struct Vector3D lightVector = getVector3D(surfacePoint, &lights[i].position);
        double diffuseProduct = getDotProduct(normal, &lightVector);

        intensity += (getIntensity(surfacePoint, &lights[i]) * diffuseProduct);
    };

    return intensity;
}

double calculateSpecularIntensity(struct Point3D *surfacePoint, struct Vector3D *normal, struct Vector3D *viewerVector)
{
    double intensity = 0;
    for(int i = 0; i < lightsCount; i++)
    {
        struct Vector3D lightVector = getVector3D(surfacePoint, &lights[i].position);

        double reflectedScalingFactor = 2 * getDotProduct(&lightVector, normal);
        struct Vector3D scaledNormal = getScaledVector(normal, reflectedScalingFactor);

        reverseVector3D(&lightVector);
        struct Vector3D reflectedVector = getSummaryVector3D(normal, &lightVector);

        double specularProduct = getDotProduct(&reflectedVector, viewerVector);

        intensity += (getIntensity(surfacePoint, &lights[i]) * specularProduct);
    }

    return intensity;
}