#include "../include/Shader.h"

Shader::Shader() {}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    // retrieve vertex / fragment shader source code from file path
    std::string vertexString, fragmentString;
    std::ifstream vertexFile, fragmentFile;
    std::stringstream vertexStream, fragmentStream;

    // allow ifstreams to throw exceptions
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        // read file contents into streams
        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        // close files
        vertexFile.close();
        fragmentFile.close();

        // extract strings from streams
        vertexString = vertexStream.str();
        fragmentString = fragmentStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR: SHADER FILE WAS UNSUCCESSFULLY READ" << std::endl;
        exit(-1);
    }

    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    // compile shaders
    uint vertex, fragment;
    int success;
    char infoLog[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // check for errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
    }

    // delete shaders (already linked to program and no longer needed)
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use(void) {
    glUseProgram(ID);
}

void Shader::setFloatUniform(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3Uniform(const std::string &name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4Uniform(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileErrors(uint shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR: SHADER COMPILATION ERROR OF TYPE: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR: PROGRAM LINKING ERROR OF TYPE: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}