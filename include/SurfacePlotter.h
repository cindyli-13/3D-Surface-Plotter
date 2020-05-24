#ifndef SURFACEPLOTTER_H
#define SURFACEPLOTTER_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14159265
#define e 2.71828
#define FLOAT_MIN -2147483648
#define FLOAT_MAX 2147483648

class SurfacePlotter {
    private:
        // xy grid
        std::vector<std::vector<glm::vec2>> gridPoints; // 2D array of grid x, y coordinates
        float xMin;
        float xMax;
        float yMin;
        float yMax;
        float gridInterval;
        float zMin;
        float zMax;

        // surface plot data
        float* vertices;
        uint numElements;
        uint* indices;
        uint numIndices;

    public:
        SurfacePlotter();

        void setGrid(float xMin, float xMax, float yMin, float yMax, float interval);
        void generateSurfacePlot(float time);
        float f(float x, float y); // mathematical multi-variable function, returns z value

        float getZMin(void);
        float getZMax(void);
        float getZRange(void);

        float* getVertices(void);
        uint getNumElements(void);
        uint* getIndices(void);
        uint getNumIndices(void);
};

#endif //SURFACEPLOTTER_H
