#pragma once

#include "hudRect.h"

class Compass {
public:

    glm::mat4 model = glm::mat4(1.0f);

    Compass(int display_w, int display_h, Shader shader) {
        compass = new hudRect(290.0f, 290.0f, (float)(display_w - 290)/2.0f, 50.0f,
                              "./res/hud/compass.png", shader);
        arrow = new hudRect(83.0f, 39.0f, (float)(display_w - 83)/2.0f, 50.0f + compass->h + 10.0f,
                            "./res/hud/arrow.png", shader);
    }

    void Draw(Shader shader, Camera_Facing_Direction dir) {

        //XD debilu
        compass->model = glm::rotate(glm::mat4(1.0f), 90.0f * (int)dir, glm::vec3(0.0f, 0.0f, 1.0f));

        compass->Draw(shader);
        arrow->Draw(shader);
    }



private:

    hudRect* compass;
    hudRect* arrow;
};
