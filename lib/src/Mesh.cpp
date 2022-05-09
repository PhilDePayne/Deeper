//
// Created by Wojtek on 06.05.2022.
//

#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, glm::mat4 modelMatrix, GLuint texturesSetID)
{
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::modelMatrix = modelMatrix;
    Mesh::textureSetId = texturesSetID;
    setupMesh();
}

void Mesh::Draw(Shader &shader)
{
    // pass model matrix to the Shader
    glm::mat4 tmpModelMatrix = modelMatrix;
    tmpModelMatrix = glm::translate(tmpModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    tmpModelMatrix = glm::scale(tmpModelMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
    shader.setMat4("model", tmpModelMatrix);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // TexCoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

GLuint Mesh::getTexturesSetId() const
{
    return textureSetId;
}
