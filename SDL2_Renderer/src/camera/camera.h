#include "../common/geometry.h"

#ifndef SDL2_RENDERER_CAMERA_H
#define SDL2_RENDERER_CAMERA_H

typedef struct Camera Camera;

struct Camera {
    struct Point3D position;
    struct Point3D target;

    struct Vector3D direction;

    struct Vector3D sensorYVector;
    struct Vector3D sensorXVector;

    double sensorWidth;
    double sensorHeight;

    int screenWidth;
    int screenHeight;

    double focalLength;
};

void debugCamera();
void initCamera(double sensorWidth, double sensorHeight, int screenWidth, int screenHeight, double focalLength);
void setCameraPosition(struct Point3D *position);
void setCameraTarget(struct Point3D *target);
void logCamera();
struct Point3D getScreenPointPosition(int x, int y);
struct Vector3D getScreenPointDirection(int x, int y);
struct Point3D getCameraPosition();
struct Point3D getCameraTarget();
#endif //SDL2_RENDERER_CAMERA_H
