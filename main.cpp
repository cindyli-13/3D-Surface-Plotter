#include "SurfacePlotter.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1200

const char* vertexShaderPath = "vertexShader.vs";
const char* fragmentShaderPath = "fragmentShader.fs";

int main() {
    SurfacePlotter program;
    program.init(WINDOW_WIDTH, WINDOW_HEIGHT, vertexShaderPath, fragmentShaderPath);
    program.setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    program.run();
    program.cleanup();
    return 0;
}