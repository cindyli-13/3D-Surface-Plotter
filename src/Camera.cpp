#include "../include/Camera.h"

// default constructor
Camera::Camera() :
    position(0.0f, 0.0f,  30.0f),
    front(0.0f, 0.0f, -0.5f),
    up(1.0f, 0.0f,  0.0f),
    right(0.0f, -1.0f, 0.0f),
    movementSpeed(10.0f),
    zoom(45.0f),
    zoomSpeed(0.5f)
    {}

glm::mat4 Camera::getViewMatrix(void) {
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

glm::mat4 Camera::getProjectionMatrix(float screenWidth, float screenHeight) {

}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = this->movementSpeed * deltaTime;
    switch (direction) {
        case FORWARD:
            position += up * velocity;
            break;
        case BACKWARD:
            position -= up * velocity;
            break;
        case LEFT:
            position -= right * velocity;
            break;
        case RIGHT:
            position += right * velocity;
            break;
    }
}

void Camera::processMouseScroll(float yOffset) {
    this->zoom -= (float) yOffset * zoomSpeed;

    // clamp between max and min zoom values
    if (zoom < MIN_ZOOM)
        zoom = MIN_ZOOM;
    if (zoom > MAX_ZOOM)
        zoom = MAX_ZOOM;
}