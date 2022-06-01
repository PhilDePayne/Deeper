#pragma once

#include "Texture.h"
#include <vector>

class Compass {
public:

    glm::mat4 model = glm::mat4(1.0f);

    Compass(char *path, Shader shader) {

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

        model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f));
    }

    void Draw(Shader shader, int display_w, int display_h, float rotation) {

        if(oldW != display_w || oldH != display_h) {
            oldW = display_w;
            oldH = display_h;
            adjustVertices();
            rotate(rotation);
        }



        shader.use();
        shader.setMat4("model", model);
        texture->texUnit(shader, "tex0", 0);
        texture->Bind();
        glBindVertexArray(VAO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertices.size(), &vertices.front(), GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void adjustVertices() {

        x = -oldW/2.0f + 100.0f;
        y = -oldH/2.0f + 100.0f;
        ed = 70.0f * oldW/1080.0f;

        vertices = {
            //COORDINATES           TEXTURE
            //back
            x, 		y, 	    z,    	1.0f,  0.0f,
            x+ed, 	y, 	    z,     	0.0f,  0.0f,
            x+ed,  	y+ed, 	z,     	0.0f,  1.0f,
            x+ed,  	y+ed, 	z,     	0.0f,  1.0f,
            x,  	y+ed, 	z,    	1.0f,  1.0f,
            x, 		y, 	    z,    	1.0f,  0.0f,

                //front
            x, 		y,  	z+ed,	0.0f,  0.0f,
            x+ed, 	y,  	z+ed,	1.0f,  0.0f,
            x+ed,  	y+ed,  	z+ed,	1.0f,  1.0f,
            x+ed,  	y+ed,  	z+ed,	1.0f,  1.0f,
            x,  	y+ed,  	z+ed,	0.0f,  1.0f,
            x, 		y,  	z+ed,	0.0f,  0.0f,

                //TODO: dokoncz koordy tekstur
                //left
            x,  	y+ed,  	z+ed,	-1.0f,  0.0f,
            x,  	y+ed, 	z,	    -1.0f,  0.0f,
            x, 		y, 	    z,	    -1.0f,  0.0f,
            x, 		y, 	    z,	    -1.0f,  0.0f,
            x, 		y,  	z+ed,	-1.0f,  0.0f,
            x,  	y+ed,  	z+ed,	-1.0f,  0.0f,

            x+ed,  	y+ed,  	z+ed,	1.0f,  0.0f,
            x+ed,  	y+ed, 	z,	    1.0f,  0.0f,
            x+ed, 	y, 	    z,	    1.0f,  0.0f,
            x+ed, 	y, 	    z,	    1.0f,  0.0f,
            x+ed, 	y,  	z+ed,	1.0f,  0.0f,
            x+ed,  	y+ed,  	z+ed,	1.0f,  0.0f,

            x, 		y, 	    z,    	0.0f, -1.0f,
            x+ed, 	y, 	    z,	    0.0f, -1.0f,
            x+ed, 	y,  	z+ed,	0.0f, -1.0f,
            x+ed, 	y,  	z+ed,	0.0f, -1.0f,
            x, 		y,  	z+ed,	0.0f, -1.0f,
            x, 		y, 	    z,	    0.0f, -1.0f,

            x,  	y+ed, 	z,	    0.0f,  1.0f,
            x+ed,  	y+ed, 	z,	    0.0f,  1.0f,
            x+ed,  	y+ed,  	z+ed,	0.0f,  1.0f,
            x+ed,  	y+ed,  	z+ed,	0.0f,  1.0f,
            x,  	y+ed,  	z+ed,	0.0f,  1.0f,
            x,  	y+ed, 	z,	    0.0f,  1.0f
        };

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void rotate(float rotation) {
       model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }


private:
    unsigned int VAO;
    unsigned int VBO;

    Texture* texture;

    int oldW = 0.0f, oldH = 0.0f;
    float x = -oldW/2.0f, y = -oldH/2.0f, z = -50.0f, ed = 100.0f;

    std::vector<float> vertices {
            //COORDINATES           TEXTURE
            //back
            x, 		y, 	    z,    	1.0f,  0.0f,
            x+ed, 	y, 	    z,     	0.0f,  0.0f,
            x+ed,  	y+ed, 	z,     	0.0f,  1.0f,
            x+ed,  	y+ed, 	z,     	0.0f,  1.0f,
            x,  	y+ed, 	z,    	1.0f,  1.0f,
            x, 		y, 	    z,    	1.0f,  0.0f,

            //front
            x, 		y,  	z+ed,	0.0f,  0.0f,
            x+ed, 	y,  	z+ed,	1.0f,  0.0f,
            x+ed,  	y+ed,  	z+ed,	1.0f,  1.0f,
            x+ed,  	y+ed,  	z+ed,	1.0f,  1.0f,
            x,  	y+ed,  	z+ed,	0.0f,  1.0f,
            x, 		y,  	z+ed,	0.0f,  0.0f,

            //TODO: dokoncz koordy tekstur
            //left
            x,  	y+ed,  	z+ed,	-1.0f,  0.0f,
            x,  	y+ed, 	z,	    -1.0f,  0.0f,
            x, 		y, 	    z,	    -1.0f,  0.0f,
            x, 		y, 	    z,	    -1.0f,  0.0f,
            x, 		y,  	z+ed,	-1.0f,  0.0f,
            x,  	y+ed,  	z+ed,	-1.0f,  0.0f,

            x+ed,  	y+ed,  	z+ed,	1.0f,  0.0f,
            x+ed,  	y+ed, 	z,	    1.0f,  0.0f,
            x+ed, 	y, 	    z,	    1.0f,  0.0f,
            x+ed, 	y, 	    z,	    1.0f,  0.0f,
            x+ed, 	y,  	z+ed,	1.0f,  0.0f,
            x+ed,  	y+ed,  	z+ed,	1.0f,  0.0f,

            x, 		y, 	    z,    	0.0f, -1.0f,
            x+ed, 	y, 	    z,	    0.0f, -1.0f,
            x+ed, 	y,  	z+ed,	0.0f, -1.0f,
            x+ed, 	y,  	z+ed,	0.0f, -1.0f,
            x, 		y,  	z+ed,	0.0f, -1.0f,
            x, 		y, 	    z,	    0.0f, -1.0f,

            x,  	y+ed, 	z,	    0.0f,  1.0f,
            x+ed,  	y+ed, 	z,	    0.0f,  1.0f,
            x+ed,  	y+ed,  	z+ed,	0.0f,  1.0f,
            x+ed,  	y+ed,  	z+ed,	0.0f,  1.0f,
            x,  	y+ed,  	z+ed,	0.0f,  1.0f,
            x,  	y+ed, 	z,	    0.0f,  1.0f
    };

};
