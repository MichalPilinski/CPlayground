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
}

void calculateCameraInternals()
{
    calculateCameraDirection();
    calculateSensorNormals();
}

void setCameraPosition(struct Point3D *position)
{
    _camera.position = *position;

    calculateCameraInternals();
}

void setCameraTarget(struct Point3D *target)
{
    _camera.target = *target;

    calculateCameraInternals();
}

void initCamera(double sensorWidth, double sensorHeight, int screenWidth, int screenHeight, double focalLength)
{
    _camera.sensorHeight = sensorHeight;
    _camera.sensorWidth = sensorWidth;

    _camera.screenHeight = screenHeight;
    _camera.screenWidth = screenWidth;

    _camera.focalLength = focalLength;

    calculateCameraInternals();
}

/// Computes camera-sensor_point direction
/// \param x Number of pixels from left side of the screen
/// \param y Number of pixels from upper side of the screen
/// \return Vector pointing from camera to given pixel on sensor
struct Vector3D getScreenPointDirection(int x, int y)
{
    struct Vector3D centerToSensorVector = getScaledVector(&_camera.direction, _camera.focalLength);

    double horizontalOffset = (((double)x / _camera.screenWidth) - 0.5f) * _camera.sensorWidth;
    double verticalOffset = (((double)y / _camera.screenHeight) - 0.5f) * _camera.sensorHeight;

    struct Vector3D horizontalVector = getScaledVector(&_camera.sensorXVector, horizontalOffset);
    struct Vector3D verticalVector = getScaledVector(&_camera.sensorYVector, verticalOffset);

    addToVector3D(&centerToSensorVector, &horizontalVector);
    addToVector3D(&centerToSensorVector, &verticalVector);

    return centerToSensorVector;
}

struct Point3D getScreenPointPosition(int x, int y)
{
    struct Vector3D centerToSensorVector = getScreenPointDirection(x, y);

    struct Point3D sensorPoint = _camera.position;
    addVectorToPoint(&sensorPoint, &centerToSensorVector);

    return sensorPoint;
}

struct Point3D getCameraPosition()
{
    return _camera.position;
}

struct Point3D getCameraTarget()
{
    return _camera.target;
}

void logCamera() {
    printf("||Camera log||\n");

    printf("Target: ");
    logPoint3D(&_camera.target);
    printf("\n");

    printf("Position: ");
    logPoint3D(&_camera.position);
    printf("\n");

    printf("Direction: ");
    logVector3D(&_camera.direction);
    printf("\n");

    printf("Sensor size: [%f, %f]", _camera.sensorWidth, _camera.sensorHeight);
    printf("\n");

    printf("Screen size: [%d, %d]", _camera.screenWidth, _camera.screenHeight);
    printf("\n");

    printf("Focal length: %f\n", _camera.focalLength);
    printf("\n");

}

void debugCamera()
{
    initCamera(1,1, 1000, 1000, 1);
    setCameraPosition(&(struct Point3D){0, 0, 0});
    setCameraTarget(&(struct Point3D){5, 5 ,5});

    logCamera();

    struct Point3D debugPoint = getScreenPointPosition(0, 1000);
    logPoint3D(&debugPoint);

    setCameraTarget(&(struct Point3D){5, 0, 0});

    struct Point3D debugPoint2 = getScreenPointPosition(0, 1000);
    logPoint3D(&debugPoint2);
}
