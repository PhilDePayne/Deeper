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

    unsigned int getVAO();

    unsigned int getVBO();

    void render();

    bool isType(ComponentType t) override;

    void test() override;

private:

    std::vector<float> vertices;

    unsigned int VAO;
    unsigned int VBO;

    float radius = 1.0f;

    int sectorCount = 12;
    int stackCount = 12;

    const double PI = 3.141592653589793238463;

};