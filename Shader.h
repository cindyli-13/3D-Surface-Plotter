#ifndef INC_3DSURFACEPLOTTER_SHADER_H
#define INC_3DSURFACEPLOTTER_SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
    public:
        uint ID;
        Shader();
        Shader(const char* vertexPath, const char* fragmentPath);
        void use(void);
        void setVec3Uniform(const std::string &name, glm::vec3 value) const;
        void setMat4Uniform(const std::string &name, glm::mat4 value) const;

    private:
        void checkCompileErrors(uint shader, std::string type);
};

#endif //INC_3DSURFACEPLOTTER_SHADER_H
