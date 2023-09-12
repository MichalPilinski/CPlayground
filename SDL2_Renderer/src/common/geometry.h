#ifndef SDL2_RENDERER_GEOMETRY_H
#define SDL2_RENDERER_GEOMETRY_H

#include <math.h>

struct Point3D {
    double x;
    double y;
    double z;
};

struct Vector3D {
    double x;
    double y;
    double z;
};

void scaleVector3D(struct Vector3D *vector, double factor);
struct Vector3D getScaledVector(struct Vector3D *vector, double factor);
void addToVector3D(struct Vector3D *targetVector, struct Vector3D *sourceVector);
struct Vector3D getSummaryVector3D(struct Vector3D *vec1, struct Vector3D *vec2);
void reverseVector3D(struct Vector3D *vector);
void normalizeVector3D(struct Vector3D *vector);
struct Vector3D getVector3D(struct Point3D *pointFrom, struct Point3D *pointTo);
double getDotProduct(struct Vector3D *vec1, struct Vector3D *vec2);
double getVector3DLength(struct Vector3D *vector);
struct Vector3D getDotProductVector3D(struct Vector3D *vec1, struct Vector3D *vec2);
void clampVector3D(struct Vector3D *vec);

void addVectorToPoint(struct Point3D *point, struct Vector3D *vector3D);
double getPointsDistance(struct Point3D *point, struct Point3D *point2);

void logVector3D(struct Vector3D *vector);
void logPoint3D(struct Point3D *point);

#endif //SDL2_RENDERER_GEOMETRY_H
