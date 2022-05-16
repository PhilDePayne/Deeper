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
    for (auto i : vertices) {

        if(i.Position.x > maxX) maxX = i.Position.x;
        if (i.Position.x < minX) minX = i.Position.x;
        if (i.Position.z > maxY) maxY = i.Position.z;
        if (i.Position.z < minY) minY = i.Position.z;
        if (i.Position.y > maxZ) maxZ = i.Position.y;
        if (i.Position.y < minZ) minZ = i.Position.y;

        //printf("%f %f %f\n", i.Position.x, i.Position.y, i.Position.z);

    }

    printf("%f %f %f %f %f %f\n", maxX, minX, maxY, minY, maxZ, minZ);

    boundingVolume.setCenter(glm::vec3((maxX + minX) / 2 , (maxY + minY) / 2 , (maxZ + minZ) / 2 ));
    boundingVolume.setSize(glm::vec3((maxX - minX) , (maxY - minY) , (maxZ - minZ) ));

    printf("%f %f %f %f %f %f\n", boundingVolume.getCenter().x, boundingVolume.getCenter().y, boundingVolume.getCenter().z, 
        boundingVolume.getSizeX(), boundingVolume.getSizeY(), boundingVolume.getSizeZ());
}

void Mesh::Draw(Shader &shader, Transform modelTransform)
{
    // pass model matrix to the Shader
    glm::mat4 tmpModelMatrix = modelMatrix;
    tmpModelMatrix = modelTransform.getCombinedMatrix() * tmpModelMatrix;
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
