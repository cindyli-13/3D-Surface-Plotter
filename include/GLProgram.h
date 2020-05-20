#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "SurfacePlotter.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLProgram {
    private:
        GLFWwindow* window;
        int windowWidth, windowHeight;

        float deltaTime, prevTime;

        struct color {
            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;
            float alpha = 1.0f;
        } clearColor;

        Shader shader;
        SurfacePlotter surfacePlotter;
        uint surfacePlotVAO, surfacePlotVBO, surfacePlotEBO;

        void initDrawingData(void);

    public:
        static Camera camera;

        GLProgram(int windowWidth, int windowHeight);

        void init(const char* vertexPath, const char* fragmentPath);
        void run(void);
        void cleanup(void);

        void setClearColor(float r, float g, float b, float alpha);

        uint generateBuffer(void);
        uint generateVAO(void);

        void drawSurfacePlot(void);

        // transformation matrices
        glm::mat4 getViewMatrix(void);
        glm::mat4 getProjectionMatrix(void);
        glm::mat4 getModelMatrix(void);

        // call back for when the viewport is resized
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        // input
        void processInput(void);
        void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif //GLPROGRAM_H
