#include <printf.h>
#include "geometry.h"

// region Debug
void logVector3D(struct Vector3D *vector)
{
    printf("[%f, %f, %f]", vector->x, vector->y, vector->z);
}

void logPoint3D(struct Point3D *point)
{
    printf("[%f, %f, %f]", point->x, point->y, point->z);
}
// endregion

// region Point
void addVectorToPoint(struct Point3D *point, struct Vector3D *vector3D)
{
    point->x = point->x + vector3D->x;
    point->y = point->y + vector3D->y;
    point->z = point->z + vector3D->z;
}

double getPointsDistance(struct Point3D *point, struct Point3D *point2)
{
    return sqrt(pow(point->x - point2->x, 2) + pow(point->y - point2->y, 2) + pow(point->z - point2->z, 2));
}
// endregion

// region Vector
void scaleVector3D(struct Vector3D *vector, double factor)
{
    vector->x *= factor;
    vector->y *= factor;
    vector->z *= factor;
}

struct Vector3D getScaledVector(struct Vector3D *vector, double factor)
{
    return (struct Vector3D) {
        .x = vector->x * factor,
        .y = vector->y * factor,
        .z = vector->z * factor
    };
}

void addToVector3D(struct Vector3D *targetVector, struct Vector3D *sourceVector) {
    targetVector->x += sourceVector->x;
    targetVector->y += sourceVector->y;
    targetVector->z += sourceVector->z;
}

struct Vector3D getSummaryVector3D(struct Vector3D *vec1, struct Vector3D *vec2)
{
    return (struct Vector3D) {
        .x = vec1->x + vec2->x,
        .y = vec1->y + vec2->y,
        .z = vec1->z + vec2->z
    };
}

void reverseVector3D(struct Vector3D *vector)
{
    vector->x *= -1;
    vector->y *= -1;
    vector->z *= -1;
}

struct Vector3D getVector3D(struct Point3D *pointFrom, struct Point3D *pointTo)
{
    struct Vector3D vector = {
        .x = pointTo->x - pointFrom->x,
        .y = pointTo->y - pointFrom->y,
        .z = pointTo->z - pointFrom->z
    };

    normalizeVector3D(&vector);

    return vector;
}

double getVector3DLength(struct Vector3D *vector)
{
    return sqrt(
        (vector->x * vector->x) +
        (vector->y * vector->y) +
        (vector->z * vector->z)
    );
}

void normalizeVector3D(struct Vector3D *vector)
{
    double vectorLength = getVector3DLength(vector);

    vector->x /= vectorLength;
    vector->y /= vectorLength;
    vector->z /= vectorLength;
}

double getDotProduct(struct Vector3D *vec1, struct Vector3D *vec2)
{
    return (vec1->x * vec2->x) +
           (vec1->y * vec2->y) +
           (vec1->z * vec2->z);
}

struct Vector3D getDotProductVector3D(struct Vector3D *vec1, struct Vector3D *vec2)
{
    return (struct Vector3D) {
        .x = (vec1->y * vec2->z) - (vec1->z * vec2->y),
        .y = (vec1->z * vec2->x) - (vec1->x * vec2->z),
        .z = (vec1->x * vec2->y) - (vec1->y * vec2->x),
    };
}

void clampVector3D(struct Vector3D *vec)
{
    if(vec->x < 0) vec->x = 0;
    if(vec->y < 0) vec->y = 0;
    if(vec->z < 0) vec->z = 0;
}
// endregion