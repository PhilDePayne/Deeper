#pragma once

#include "Texture.h"

#include <vector>
#include <cstdio>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

class hudRect {
public:

    glm::mat4 model = glm::mat4(1.0f);
    float w = 0.0f, h = 0.0f;
    float x = 0.0f, y = 0.0f;

    hudRect(float w, float h, float x, float y, char* path, Shader shader) : w(w), h(h), x(x), y(y) {
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        shader.use();
        texture = new Texture(path, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        texture->texUnit(shader, "tex0", 0);
    }

    void Draw(Shader shader) {
        shader.use();
        shader.setMat4("model", model);
        texture->texUnit(shader, "tex0", 0);
        texture->Bind();
        glBindVertexArray(VAO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertices.size(), &vertices.front(), GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void setCoords(float _w, float _h, float _x, float _y) {
        w = _w;
        h = _h;
        x = _x;
        y = _y;

        std::vector<float> vertices {
                //COORDINATES           TEXTURE COORDS
                x,      y,      0.0f,   0.0f, 0.0f,
                x,      y + h,  0.0f,   0.0f, 1.0f,
                x + w,  y + h,  0.0f,   1.0f, 1.0f,

                x,      y,      0.0f,   0.0f, 0.0f,
                x + w,  y + h,  0.0f,   1.0f, 1.0f,
                x + w,  y,      0.0f,   1.0f, 0.0f
        };

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    const std::vector<float> &getVertices() const {
        return vertices;
    }

    int getState() const {
        return state;
    }

    void setState(int _state) {
        hudRect::state = _state;
    }

private:

    std::vector<float> vertices {
        //COORDINATES           TEXTURE COORDS
        x,      y,      0.0f,   0.0f, 0.0f,
        x,      y + h,  0.0f,   0.0f, 1.0f,
        x + w,  y + h,  0.0f,   1.0f, 1.0f,

        x,      y,      0.0f,   0.0f, 0.0f,
        x + w,  y + h,  0.0f,   1.0f, 1.0f,
        x + w,  y,      0.0f,   1.0f, 0.0f
    };

    unsigned int VAO;
    unsigned int VBO;

    Texture* texture;
    int state = -1;

};
