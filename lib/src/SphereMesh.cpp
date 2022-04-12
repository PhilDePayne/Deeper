#include "SphereMesh.h"

SphereMesh::SphereMesh() {

    vertices.resize(sectorCount * stackCount * 3);

    float x, y, z, xz;

    float sliceAngle, stackAngle;

    std::vector<GLfloat>::iterator itV = vertices.begin();

    for (int i = 0; i < stackCount; i++)
    {
        stackAngle = PI_2 - PI * i / stackCount;
        xz = radius * cosf(stackAngle);
        y = radius * sinf(stackAngle);

        for (int j = 0; j < sectorCount; j++)
        {
            sliceAngle = 2 * PI * j / sectorCount;

            x = xz * sinf(sliceAngle);
            z = xz * cosf(sliceAngle);
            *itV++ = x;
            *itV++ = y;
            *itV++ = z;

        }
    }

    int first, second;
    for (int i = 0; i < stackCount; i++)
    {
        for (int j = 0; j < sectorCount; j++)
        {
            first = i * sectorCount + j;
            second = first + sectorCount + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), &indices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

};

SphereMesh::~SphereMesh() {
};

unsigned int SphereMesh::getVAO() {

    return VAO;

};

unsigned int SphereMesh::getVBO() {

    return VBO;

}

unsigned int SphereMesh::getEBO() {

    return EBO;

}

std::vector<float> SphereMesh::getVertices() {

    return vertices;

}

std::vector<float> SphereMesh::getIndices() {

    return indices;

}

bool SphereMesh::isType(ComponentType t) {

    return (t == ComponentType::SPHEREMESH);

}

void SphereMesh::test() {

    printf("TEST_SPHEREMESH\n");

}

