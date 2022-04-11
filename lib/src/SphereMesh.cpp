#include "SphereMesh.h"

SphereMesh::SphereMesh() {

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

        }
    }

};

SphereMesh::~SphereMesh() {
};

unsigned int SphereMesh::getVAO() {

    return VAO;

};

unsigned int SphereMesh::getVBO() {

    return VBO;

}

std::vector<float> SphereMesh::getVertices() {

    return vertices;

}

bool SphereMesh::isType(ComponentType t) {

    return (t == ComponentType::SPHEREMESH);

}

void SphereMesh::test() {

    printf("TEST_SPHEREMESH\n");

}

