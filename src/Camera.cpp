#include "../include/Camera.h"

// default constructor
Camera::Camera() :
    position(0.0f, 0.0f,  30.0f),
    front(0.0f, 0.0f, -0.5f),
    up(1.0f, 0.0f,  0.0f),
    right(0.0f, -1.0f, 0.0f),
    movementSpeed(15.0f),
    zoom(45.0f),
    zoomSpeed(2.0f)
    {}

glm::mat4 Camera::getViewMatrix(void) {
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = this->movementSpeed * deltaTime;
    switch (direction) {
        case UP:
            this->position += this->up * velocity;
            break;
        case DOWN:
            this->position -= this->up * velocity;
            break;
        case LEFT:
            this->position -= this->right * velocity;
            break;
        case RIGHT:
            this->position += this->right * velocity;
            break;
    }
}

void Camera::processMouseScroll(float yOffset) {
    this->zoom -= (float) yOffset * this->zoomSpeed;

    // clamp between max and min zoom values
    if (this->zoom < MIN_ZOOM)
        this->zoom = MIN_ZOOM;
    if (this->zoom > MAX_ZOOM)
        this->zoom = MAX_ZOOM;
}