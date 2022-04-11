#pragma once

#include <glad/glad.h>

#include "Component.h"
#include "Shader.h"

#include <vector>

class SphereMesh : public Component {

public:
	SphereMesh();
	~SphereMesh();

    std::vector<float> getVertices();

    std::vector<float> getIndices();

    unsigned int getVAO();

    unsigned int getVBO();

    unsigned int getEBO();

    void render();

    bool isType(ComponentType t) override;

    void test() override;

private:

    std::vector<float> vertices;
    std::vector<float> indices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    float radius = 1.0f;

    int sectorCount = 60;
    int stackCount = 60;

    const double PI = 3.141592653589793238463;
    const double PI_2 = 1.57079632679489661923;

};