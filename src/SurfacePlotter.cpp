#include "../include/SurfacePlotter.h"

// default constructor
SurfacePlotter::SurfacePlotter() : xMin(-3.0f), xMax(3.0f), yMin(-3.0f), yMax(3.0f), gridInterval(0.1f), vertices(NULL), numElements(0), indices(NULL), numIndices(0), zMin(FLOAT_MAX), zMax(FLOAT_MIN) {
    setGrid(this->xMin, this->xMax, this->yMin, this->yMax, this->gridInterval);
}

void SurfacePlotter::setGrid(float xMin, float xMax, float yMin, float yMax, float interval) {
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    this->gridInterval = interval;

    // empty grid points array
    this->gridPoints.clear();

    // fill grid points array
    for (float x = xMin; x <= xMax; x += interval) {
        std::vector<glm::vec2> temp;
        this->gridPoints.push_back(temp);
        for (float y = yMin; y <= yMax; y += interval) {
            this->gridPoints[this->gridPoints.size()-1].push_back(glm::vec2(x, y));
        }
    }
}

void SurfacePlotter::generateSurfacePlot(float time) {

    // reset ranges
    zMin = FLOAT_MAX;
    zMax = FLOAT_MIN;

    // empty grid
    if (gridPoints.empty())
        return;

    // vertices:

    // deallocate old data
    if (this->vertices)
        delete[] vertices;

    // determine number of rows in x and y axes
    int numX = this->gridPoints.size();
    int numY = this->gridPoints[0].size();

    // allocate memory for new data
    this->numElements = 3 * numX * numY;
    this->vertices = new float[this->numElements];

    // generate vertices
    for (int x = 0; x < numX; ++x) {
        for (int y = 0; y < numY; ++y) {

            // add vertex
            this->vertices[(x*numY + y) * 3 + 0] = this->gridPoints[x][y].x; // x
            this->vertices[(x*numY + y) * 3 + 1] = this->gridPoints[x][y].y; // y
            // this->vertices[(x*numY + y) * 3 + 2] = sin(2*time) * f(this->gridPoints[x][y].x, this->gridPoints[x][y].y); // z time-dependent
            this->vertices[(x*numY + y) * 3 + 2] = f(this->gridPoints[x][y].x, this->gridPoints[x][y].y); // z static
        }
    }

    // indices:

    // deallocte old data
    if (this->indices)
        delete[] indices;

    // determine number of indices
    this->numIndices = (numX-1) * (numY-1) * 6; // num quads * 6

    // allocate memory for new data
    this->indices = new uint[this->numIndices];

    // generate indices
    for (int x = 0; x < numX-1; ++x) {
        for (int y = 0; y < numY-1; ++y) {

            this->indices[(x*(numY-1) + y) * 6 + 0] = x*numY + y;
            this->indices[(x*(numY-1) + y) * 6 + 1] = (x+1)*numY + y;
            this->indices[(x*(numY-1) + y) * 6 + 2] = (x+1)*numY + y+1;

            this->indices[(x*(numY-1) + y) * 6 + 3] = x*numY + y;
            this->indices[(x*(numY-1) + y) * 6 + 4] = (x+1)*numY + y+1;
            this->indices[(x*(numY-1) + y) * 6 + 5] = x*numY + y+1;
        }
    }
}

float SurfacePlotter::f(float x, float y) {

    // EQUATION
    //float z = 8*sin(sqrt(pow(x, 2) + pow(y, 2))) / sqrt(pow(x, 2) + pow(y, 2)); // sombrero equation
    //float z = sin(pow(x/2.5, 2) + pow(y/2.5, 2));
    float z = pow(x/1.5,2) + pow(y/1.5,2); // parabaloid

    // update z ranges
    if (z < zMin)
        zMin = z;
    if (z > zMax)
        zMax = z;

    return z;
}

float SurfacePlotter::getZMin(void) {
    return this->zMin;
}

float SurfacePlotter::getZMax(void) {
    return this->zMax;
}

float SurfacePlotter::getZRange(void) {
    return this->zMax - this->zMin;
}

float* SurfacePlotter::getVertices(void) {
    return this->vertices;
}

uint SurfacePlotter::getNumElements(void) {
    return this->numElements;
}

uint* SurfacePlotter::getIndices(void) {
    return this->indices;
}

uint SurfacePlotter::getNumIndices(void) {
    return this->numIndices;
}