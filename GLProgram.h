#ifndef INC_3DSURFACEPLOTTER_GLPROGRAM_H
#define INC_3DSURFACEPLOTTER_GLPROGRAM_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLProgram {
    private:
        GLFWwindow* window;
        struct color {
            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;
            float alpha = 1.0f;
        } clearColor;

        Shader shader;

    public:
        void init(int windowWidth, int windowHeight, const char* vertexPath, const char* fragmentPath);
        void run(void);
        void cleanup(void);

        void setClearColor(float r, float g, float b, float alpha);

        // call back for when the viewport is resized
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

        // input
        void processInput(void);
        void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif //INC_3DSURFACEPLOTTER_GLPROGRAM_H
