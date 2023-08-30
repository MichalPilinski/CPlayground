#include "../common/geometry.h"

#ifndef SDL2_RENDERER_CAMERA_H
#define SDL2_RENDERER_CAMERA_H

struct Camera {
    struct Point3D position;
    struct Point3D target;

    struct Vector3D direction;

    struct Vector3D sensorYVector;
    struct Vector3D sensorXVector;
    double sensorWidth;
    double sensorHeight;
};

void debugCamera();

#endif //SDL2_RENDERER_CAMERA_H
