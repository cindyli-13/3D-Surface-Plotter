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

#define MIN(a, b) ((a) < (b)) ? (a) : (b)

class GLProgram {
    private:
        GLFWwindow* window;

        float deltaTime, prevTime;

        struct color {
            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;
            float alpha = 1.0f;
        } clearColor;

        Shader shader, whiteShader;
        SurfacePlotter surfacePlotter;
        uint surfacePlotVAO, surfacePlotVBO, surfacePlotEBO;
        uint cubeVAO, cubeVBO, cubeEBO;

        void initDrawingData(void);
        static glm::vec3 getArcballVector(float x, float y); // helper to cursor callback, (x,y) are raw mouse coordinates

    public:
        static int windowWidth, windowHeight;
        static Camera camera;
        static bool mousePressed;
        static double prevMouseX, prevMouseY;
        static glm::mat4 modelMatrix;

        GLProgram();

        void init(const char* vertexPath, const char* fragmentPath, const char* whiteFragmentPath);
        void run(void);
        void cleanup(void);

        void setClearColor(float r, float g, float b, float alpha);

        uint generateBuffer(void);
        uint generateVAO(void);

        void drawSurfacePlot(void);
        void drawCube(void);

        // transformation matrices
        glm::mat4 getViewMatrix(void);
        glm::mat4 getProjectionMatrix(void);
        glm::mat4 getDefaultModelMatrix(void);

        // event callback functions
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

        // input
        void processInput(void);
};

#endif //GLPROGRAM_H
