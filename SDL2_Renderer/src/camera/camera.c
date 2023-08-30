#include <printf.h>

#include "camera.h"

struct Camera _camera;

void calculateCameraDirection()
{
    struct Vector3D targetPositionVector = getVector3D(&_camera.position, &_camera.target);
    normalizeVector3D(&targetPositionVector);

    _camera.direction = targetPositionVector;
}

void calculateSensorNormals()
{
    struct Vector3D worldZVector = {0, 0, 1};

    double directionVectorLength = getVector3DLength(&_camera.direction);
    double zToDirectionCos = getDotProduct(&worldZVector, &_camera.direction) / directionVectorLength;

    struct Vector3D worldOnDirectionProjection = getScaledVector(&_camera.direction, zToDirectionCos);
    reverseVector3D(&worldOnDirectionProjection);

    _camera.sensorYVector = getSummaryVector3D(&worldOnDirectionProjection, &worldZVector);
    normalizeVector3D(&_camera.sensorYVector);

    _camera.sensorXVector = getDotProductVector3D(&_camera.sensorYVector, &_camera.direction);
    normalizeVector3D(&_camera.sensorXVector);

    printf("Vectorro: ");
    logVector3D(&_camera.sensorYVector);
    logVector3D(&_camera.sensorXVector);
    printf("\n");
}

void calculateCameraInternals()
{
    calculateCameraDirection();
    calculateSensorNormals();
}

void initCamera(double sensorWidth, double sensorHeight)
{
    _camera = (struct Camera) {
            .position = {0, 0, 0},
            .target = {2, 0, 2},
    };

    _camera.sensorHeight = sensorHeight;
    _camera.sensorWidth = sensorWidth;

    calculateCameraInternals();
}

void logCamera() {
    printf("Camera log:\n");

    printf("Target: ");
    logPoint3D(&_camera.target);
    printf("\n");

    printf("Position: ");
    logPoint3D(&_camera.position);
    printf("\n");

    printf("Direction: ");
    logVector3D(&_camera.direction);
    printf("\n");

    printf("Sensor sizes: [%f, %f]", _camera.sensorWidth, _camera.sensorHeight);
    printf("\n");
}

void debugCamera()
{
    initCamera(1,1);
    logCamera();
}
