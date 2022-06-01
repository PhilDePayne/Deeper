//
// Created by Wojtek on 06.05.2022.
//

#pragma once

//#include <string>
#include <vector>
//#include "glm/glm.hpp"
//#include "glad/glad.h"
//#include "shaderClass.h"
#include "Shader.h"

#include "Transform.h"
#include "BoxCollider.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh
{
public:

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, glm::mat4 modelMatrix, GLuint texturesSetId);

    void Draw(Shader &shader, Transform modelTransform);

    GLuint getTexturesSetId() const;

    float maxX = 0.0f;
    float minX = 0.0f;
    float maxY = 0.0f;
    float minY = 0.0f;
    float maxZ = 0.0f;
    float minZ = 0.0f;

    BoxCollider boundingVolume;

private:

    GLuint VAO, VBO, EBO, textureSetId;

    void setupMesh();

};