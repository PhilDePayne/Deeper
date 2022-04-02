#pragma once

#include "Component.h"

#include <glad/glad.h>

#include <vector>

class PlainMesh : public Component {

public:

	PlainMesh();
	~PlainMesh();

    std::vector<float> getVertices();

    std::vector<int> getIndices();

    unsigned int getVAO();

    unsigned int getVBO();

    unsigned int getEBO();

    bool isType(ComponentType t) override;

    void test() override;

private:

    std::vector<float> vertices = { //TODO: pos/col/tex vector
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };

    std::vector<int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};