#ifndef INC_3DSURFACEPLOTTER_SURFACEPLOTTER_H
#define INC_3DSURFACEPLOTTER_SURFACEPLOTTER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <vector>
#include "GLProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SurfacePlotter : public GLProgram {
    private:

        // xy grid
        std::vector<float> gridPoints;
        float xMin;
        float xMax;
        float yMin;
        float yMax;
        float gridInterval;

    public:

};

#endif //INC_3DSURFACEPLOTTER_SURFACEPLOTTER_H
