#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_ZOOM 100.0f
#define MIN_ZOOM 1.0f

enum CameraMovement {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Camera {
    public:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;

        float yaw;
        float pitch;
        float movementSpeed;
        float zoom;
        float zoomSpeed;

        Camera();

        glm::mat4 getViewMatrix(void);
        void processKeyboard(CameraMovement direction, float deltaTime);
        void processMouseScroll(float yOffset);
};

#endif //CAMERA_H
