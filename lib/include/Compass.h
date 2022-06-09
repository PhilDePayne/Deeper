#pragma once

#include "hudRect.h"

class Compass {
public:

    glm::mat4 model = glm::mat4(1.0f);

    Compass(int display_w, int display_h, Shader shader) {
        init(display_w, display_h, shader);
    }

    void init(int display_w, int display_h, Shader shader) {

        compass = new hudRect(216.0f, 216.0f, -216.0f/2.0f, -216.0f/2.0f,
                              "./res/hud/compass1.png", shader);
        arrow = new hudRect(47.0f, 28.0f, -47.0f/2.0f, -compass->h/2.0f,
                            "./res/hud/arrow.png", shader);

        compass->model = glm::translate(compass->model, glm::vec3(-display_w/2.0f + 216.0f/2.0f + 50.0f,
                                                                  -display_h/2.0f + 216.0f/2.0f + 50.0f, 0.0f));
        arrow->model = glm::translate(arrow->model, glm::vec3(-display_w/2.0f + 216.0f/2.0f + 50.0f,
                                                              -display_h/2.0f + 216.0f/2.0f + 50.0f + 150.0f, 0.0f));

        oldModel = compass->model;
        oldModelArrow = arrow->model;
        compass->model = glm::rotate(oldModel, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    }

    void Draw(Shader shader, float angle) {

        compass->model = glm::rotate(oldModel, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

        glClear(GL_DEPTH_BUFFER_BIT);
        glDepthMask(GL_FALSE);

        compass->Draw(shader);
        arrow->Draw(shader);

        glDepthMask(GL_TRUE);

    }

    void moveArrow(float x, float y) {
//        arrow->model = glm::translate(oldModelArrow, glm::vec3(x, y, 0.0f));
    }



private:

    hudRect* compass;
    hudRect* arrow;

    glm::mat4 oldModel;
    glm::mat4 oldModelArrow;

};
