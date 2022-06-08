#pragma once

#include "hudRect.h"

class Compass {
public:

    glm::mat4 model = glm::mat4(1.0f);

    Compass(int display_w, int display_h, Shader shader) {
//        compass = new hudRect(290.0f, 290.0f, (float)(display_w - 290)/2.0f, 50.0f,
//                              "./res/hud/compass.png", shader);
//        arrow = new hudRect(83.0f, 39.0f, (float)(display_w - 83)/2.0f, 50.0f + compass->h + 10.0f,
//                            "./res/hud/arrow.png", shader);
        init(display_w, display_h, shader);
    }

    void init(int display_w, int display_h, Shader shader) {

        compass = new hudRect(290.0f, 290.0f, -290.0f/2.0f, -290.0f/2.0f,
                              "./res/hud/compass.png", shader);
//        arrow = new hudRect(83.0f, 39.0f, -83.0f/2.0f, -compass->h/2.0f,
//                            "./res/hud/arrow.png", shader);

        compass->model = glm::translate(compass->model, glm::vec3(0.0f, -display_h/2.0f + 290.0f/2.0f + 20.0f, 0.0f));
        oldModel = compass->model;
        compass->model = glm::rotate(oldModel, glm::half_pi<float>() * (int)oldDir, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void Draw(Shader shader, Camera_Facing_Direction dir) {


        if(oldDir != dir) {
            compass->model = glm::rotate(oldModel, glm::half_pi<float>() * (int)dir, glm::vec3(0.0f, 0.0f, 1.0f));
            oldDir = dir;
        }

        compass->Draw(shader);
//        arrow->Draw(shader);
    }



private:

    hudRect* compass;
    hudRect* arrow;

    glm::mat4 oldModel;

    Camera_Facing_Direction oldDir = FRONT_DIR;


//    Model = translate(Model, vec3(1.0f, 0.0f, 0.0f));
//    Model = rotate(Model, 90.0f, vec3(1.0f, 0.0f, 0.0f));
//    Model = translate(Model, vec3(-1.0f, 0.0f, 0.0f));


//    void rotateAroundPt(int px, int py,
//                        int cx, int cy)
//    {
//        float theta = 0.0;
//        float thetaSpeed = 0.5f;
//
//            glClear(GL_COLOR_BUFFER_BIT);
//            int xf, yf;
//
//            // Update theta anticlockwise
//            // rotation
//            theta = theta + thetaSpeed;
//
//            // Check overflow
//            if (theta>= (2.0 * 3.14159))
//                theta = theta - (2.0 * 3.14159);
//
//            // actual calculations..
//            xf = (cx + (int)((float)(px - cx) *
//                             cos(theta)) - ((float)(py - cy) *
//                                            sin(theta)));
//            yf = (cy + (int)((float)(px - cx) *
//                             sin(theta)) + ((float)(py - cy) *
//                                            cos(theta)));
//
//    }
};
