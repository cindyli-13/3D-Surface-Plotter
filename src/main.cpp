#include "../include/GLProgram.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1200

const char* vertexShaderPath = "shaders/vertexShader.vs";
const char* fragmentShaderPath = "shaders/fragmentShader.fs";
Camera GLProgram::camera;

int main() {
    GLProgram program(WINDOW_WIDTH, WINDOW_HEIGHT);
    program.init(vertexShaderPath, fragmentShaderPath);
    program.setClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    program.run();
    program.cleanup();
    return 0;
}