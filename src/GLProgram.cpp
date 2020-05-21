#include "../include/GLProgram.h"

GLProgram::GLProgram(int windowWidth, int windowHeight) :
    windowWidth(windowWidth), windowHeight(windowHeight), deltaTime(0.0f), prevTime(0.0f) {}

void GLProgram::init(const char* vertexPath, const char* fragmentPath) {

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

    // initialize GLAD before making OpenGL calls
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
        exit(-1);
    }

    // GL calls
    glViewport(0, 0, this->windowWidth, this->windowHeight);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe

    // init shader
    this->shader = Shader(vertexPath, fragmentPath);

    // generate default surface plot
    this->surfacePlotter.generateSurfacePlot(1.0f);

    // set up VAOs and VBOs and EBOs
    initDrawingData();
}

// TODO
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
        this->shader.use();
        glm::mat4 viewMatrix = getViewMatrix();
        glm::mat4 projectionMatrix = getProjectionMatrix();
        glm::mat4 modelMatrix = getModelMatrix();
        int zRange = this->surfacePlotter.getZRange();
        this->shader.setFloatUniform("zRange", (zRange == 0) ? 1.0f : zRange);
        this->shader.setFloatUniform("zMin", this->surfacePlotter.getZMin());
        this->shader.setMat4Uniform("view", viewMatrix);
        this->shader.setMat4Uniform("projection", projectionMatrix);
        this->shader.setMat4Uniform("model", modelMatrix);

        // render
        this->surfacePlotter.generateSurfacePlot((float)glfwGetTime());
        drawSurfacePlot();

        // check and call events and swap buffers
        glfwSwapBuffers((this->window));
        glfwPollEvents();
    }
}

void GLProgram::initDrawingData(void) {

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

    glBindVertexArray(0);
}

void GLProgram::drawSurfacePlot(void) {
    glBindVertexArray(this->surfacePlotVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->surfacePlotVBO);
    glBufferData(GL_ARRAY_BUFFER, this->surfacePlotter.getNumElements()*sizeof(float), this->surfacePlotter.getVertices(), GL_DYNAMIC_DRAW);
    glDrawElements(GL_TRIANGLES, this->surfacePlotter.getNumIndices(),GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GLProgram::cleanup(void) {

    // clean up gl resources
    glDeleteVertexArrays(1, &(this->surfacePlotVAO));
    glDeleteBuffers(1, &(this->surfacePlotVBO));
    glDeleteBuffers(1, &this->surfacePlotEBO);

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
    return glm::perspective(glm::radians(camera.zoom), (float) this->windowWidth / (float) this->windowHeight, 0.1f, 100.0f);
}

glm::mat4 GLProgram::getModelMatrix(void) {
    //return glm::mat4(1.0f); // identity
    return glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void GLProgram::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void GLProgram::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}

void GLProgram::processInput(void) {

    // close window with 'ESC' key
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);

    // camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
}