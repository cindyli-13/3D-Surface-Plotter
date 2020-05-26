#include "../include/GLProgram.h"
#include "glm/ext.hpp"

GLProgram::GLProgram() :
    deltaTime(0.0f), prevTime(0.0f) {}

void GLProgram::init(const char* vertexPath, const char* fragmentPath, const char* whiteFragmentPath) {

    // initialize window system
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    this->window = glfwCreateWindow(this->windowWidth,this-> windowHeight, "3D Surface Plotter", NULL, NULL);
    if (this->window == NULL) {
        std::cout << "FAILED TO CREATE GLFW WINDOW" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);

    // initialize GLAD before making OpenGL calls
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
        exit(-1);
    }

    // GL calls
    glViewport(0, 0, this->windowWidth, this->windowHeight);
    glEnable(GL_DEPTH_TEST);

    // init shaders
    this->shader = Shader(vertexPath, fragmentPath);
    this->whiteShader = Shader(vertexPath, whiteFragmentPath);

    // generate default surface plot
    this->surfacePlotter.generateSurfacePlot(1.0f);

    // set up VAOs and VBOs and EBOs
    initDrawingData();
}

void GLProgram::run(void) {

    // main loop
    while (!glfwWindowShouldClose(this->window)) {

        // per-frame time logic
        float currTime = glfwGetTime();
        this->deltaTime = currTime - this->prevTime;
        this->prevTime = currTime;

        // input
        processInput();

        glClearColor(this->clearColor.r, this->clearColor.g, this->clearColor.b, this->clearColor.alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // computation
        surfacePlotter.generateSurfacePlot(1.0f);

        // set up shader and transformation matrices
        // TODO: condense this part
        glm::mat4 viewMatrix = getViewMatrix();
        glm::mat4 projectionMatrix = getProjectionMatrix();
        int zRange = this->surfacePlotter.getZRange();
        this->shader.use();
        this->shader.setFloatUniform("zRange", (zRange == 0) ? 1.0f : zRange);
        this->shader.setFloatUniform("zMin", this->surfacePlotter.getZMin());
        this->shader.setMat4Uniform("view", viewMatrix);
        this->shader.setMat4Uniform("projection", projectionMatrix);
        this->shader.setMat4Uniform("model", getDefaultModelMatrix() * modelMatrix);
        this->whiteShader.use();
        this->whiteShader.setMat4Uniform("view", viewMatrix);
        this->whiteShader.setMat4Uniform("projection", projectionMatrix);
        this->whiteShader.setMat4Uniform("model", getDefaultModelMatrix() * modelMatrix);

        // render
        this->surfacePlotter.generateSurfacePlot((float)glfwGetTime());
        drawSurfacePlot();
        drawCube();

        // check and call events and swap buffers
        glfwSwapBuffers((this->window));
        glfwPollEvents();
    }
}

void GLProgram::initDrawingData(void) {

    // SURFACE PLOT

    // generate surface plot VAO and VBO and EBO
    this->surfacePlotVAO = generateVAO();
    this->surfacePlotVBO = generateBuffer();
    this->surfacePlotEBO = generateBuffer();

    glBindVertexArray(this->surfacePlotVAO);

    // set VBO data
    glBindBuffer(GL_ARRAY_BUFFER, this->surfacePlotVBO);
    glBufferData(GL_ARRAY_BUFFER, this->surfacePlotter.getNumElements()*sizeof(float), this->surfacePlotter.getVertices(), GL_DYNAMIC_DRAW);

    // set EBO data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->surfacePlotEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->surfacePlotter.getNumIndices()*sizeof(uint), this->surfacePlotter.getIndices(), GL_DYNAMIC_DRAW);

    // vertices attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);


    // CUBE

    // generate cube VAO and VBO and EBO
    this->cubeVAO = generateVAO();
    this->cubeVBO = generateBuffer();
    this->cubeEBO = generateBuffer();

    glBindVertexArray(this->cubeVAO);

    // set VBO data
    glBindBuffer(GL_ARRAY_BUFFER, this->cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), this->surfacePlotter.getCubeVertices(), GL_STATIC_DRAW);

    // set EBO data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24*sizeof(uint), this->surfacePlotter.getCubeIndices(), GL_STATIC_DRAW);

    // vertices attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void GLProgram::drawSurfacePlot(void) {
    this->shader.use();
    glBindVertexArray(this->surfacePlotVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->surfacePlotVBO);
    glBufferData(GL_ARRAY_BUFFER, this->surfacePlotter.getNumElements()*sizeof(float), this->surfacePlotter.getVertices(), GL_DYNAMIC_DRAW);
    glDrawElements(GL_LINES, this->surfacePlotter.getNumIndices(),GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GLProgram::drawCube(void) {
    this->whiteShader.use();
    glBindVertexArray(this->cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), this->surfacePlotter.getCubeVertices(), GL_STATIC_DRAW);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GLProgram::cleanup(void) {

    // clean up gl resources
    glDeleteVertexArrays(1, &(this->surfacePlotVAO));
    glDeleteBuffers(1, &(this->surfacePlotVBO));
    glDeleteBuffers(1, &this->surfacePlotEBO);

    glDeleteVertexArrays(1, &(this->cubeVAO));
    glDeleteBuffers(1, &(this->cubeVBO));
    glDeleteBuffers(1, &this->cubeEBO);

    // clean up glfw
    glfwTerminate();
}

void GLProgram::setClearColor(float r, float g, float b, float alpha) {
    this->clearColor = {r, g, b, alpha};
}

uint GLProgram::generateBuffer(void) {
    uint buf;
    glGenBuffers(1, &buf);
    return buf;
}

uint GLProgram::generateVAO(void) {
    uint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}

glm::mat4 GLProgram::getViewMatrix(void) {
    return camera.getViewMatrix();
}

glm::mat4 GLProgram::getProjectionMatrix(void) {
    return glm::perspective(glm::radians(camera.zoom), (float) this->windowWidth / (float) this->windowHeight, 0.1f, 99999.0f);
}

glm::mat4 GLProgram::getDefaultModelMatrix(void) {
    //return glm::mat4(1.0f); // identity
    return glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void GLProgram::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

void GLProgram::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
            mousePressed = true;
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false;
        }
    }
}

void GLProgram::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}

void GLProgram::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {

    if (mousePressed) {

        // get current cursor coordinates
        double currMouseX, currMouseY;
        glfwGetCursorPos(window, &currMouseX, &currMouseY);

        // get points on arcball
        glm::vec3 va = getArcballVector(prevMouseX, prevMouseY);
        glm::vec3 vb = getArcballVector(currMouseX, currMouseY);

        float speedFactor = 0.1f;
        float angleOfRotation = speedFactor * acos(MIN(1.0f, glm::dot(va, vb)));

        // to get the axis of rotation, need to convert from camera coordinates to world coordinates
        glm::vec3 axisCamera = glm::cross(va, vb);
        glm::mat3 cameraToModel = glm::inverse(glm::mat3(camera.getViewMatrix() * modelMatrix));
        glm::vec3 axisModel = cameraToModel * axisCamera;

        // update model rotation matrix
        float tolerance = 1e-4;
        if (angleOfRotation > tolerance)
            modelMatrix = glm::rotate(modelMatrix, glm::degrees(angleOfRotation), axisModel);

        // update cursor position
        prevMouseX = currMouseX;
        prevMouseY = currMouseY;
    }
}

glm::vec3 GLProgram::getArcballVector(float x, float y) {

    // get normalized vector from center of the virtual arcball to a point P on the arcball's surface
    // if (x,y) is too far away from the arcball, return the nearest point on the arcball's surface
    glm::vec3 P(x/windowWidth * 2 - 1.0, y/windowHeight * 2 - 1.0, 0.0f);
    P.y = -P.y;

    float radius = 1.0f;
    float OP_squared = P.x * P.x + P.y * P.y;

    if (OP_squared <= radius)
        P.z = sqrt(radius - OP_squared); // apply pythagorean theorem to find z
    else
        P = glm::normalize(P); // nearest point

    return P;
}

void GLProgram::processInput(void) {

    // close window with 'ESC' key
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);

    // camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
}