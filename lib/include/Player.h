#ifndef DEEPER_PLAYER_H
#define DEEPER_PLAYER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "SceneGraphNode.h"
#include "typedefs.h"
#include "Transform.h"
#include "Model.h"
#include "Mesh.h"
#include "Camera.h"
#include "SphereCollider.h"
#include "AI.h"


class Player {
public:

    Player(char *path) {
        body = new Model(path);
        collider.setRadius(15.0f);
    }

    void render(Shader shader) {
        body->Draw(shader);
    }

    void move(GLFWwindow* window, Camera_Facing_Direction dir, float deltaTime, float &depthPos) {
        if(dir == FRONT_DIR || dir == LEFT_DIR) {
            front_or_left = -1;
        }
        else {
            front_or_left = 1;
        }

        float velocity = (float)front_or_left * deltaTime;

        //poruszanie prawo lewo i grawitacja
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//            rotate(90.0f);
            dirLR = -1;

            if (dir == FRONT_DIR || dir == BACK_DIR) {
                x = speed * velocity;
                z = 0.0f;
            } else {
                x = 0.0f;
                z = speed * velocity;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//            rotate(-90.0f);
            dirLR = 1;

            if (dir == FRONT_DIR || dir == BACK_DIR) {
                x = -speed * velocity;
                z = 0.0f;
            } else {
                x = 0.0f;
                z = -speed * velocity;
            }
        }

        //clamp velocities
        if(x != 0.0f) {
            x += velocity * 10.0f * dirLR;
            if(fabs(x) < 0.5f)
                x = 0.0f;
            if(x > 10.0f)
                x = 10.0f;
            else if(x < -10.0f)
                x = -10.0f;
        }

        if(z != 0.0f) {
            z += velocity * 10.0f * dirLR;
            if(fabs(z) < 0.5f)
                z = 0.0f;
            if(z > 10.0f)
                z = 10.0f;
            else if(z < -10.0f)
                z = -10.0f;
        }

        body->transform.position += glm::vec3(x, 0.0f, z);

//        //DEBUGUG
//        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//            if (dir == FRONT_DIR || dir == BACK_DIR) {
//                body->transform.position += glm::vec3(
//                        0.0f, 0.0f, speed * velocity);
//                    depthPos = body->transform.position.z * direction;
//
//            } else {
//                body->transform.position += glm::vec3(
//                        -speed * velocity, 0.0f, 0.0f);
//                depthPos = body->transform.position.x * -direction;
//            }
//        }
//
//        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//            if (dir == FRONT_DIR || dir == BACK_DIR) {
//
//                body->transform.position += glm::vec3(
//                        0.0f, 0.0f, -speed * velocity);
//
//                depthPos = body->transform.position.z * direction;
//
//            } else {
//                body->transform.position += glm::vec3(
//                        speed * velocity, 0.0f, 0.0f);
//
//                depthPos = body->transform.position.x * -direction;
//            }
//        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

            body->transform.position -= glm::vec3(
                0.0f, 10.0f, 0.0f);

        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

            body->transform.position += glm::vec3(
                0.0f, 10.0f, 0.0f);

        }

//        rotateLeftRight((int) dir);
//        correctAngle((int)dir);

        collider.setCenter(glm::vec3(body->transform.position.x, body->transform.position.y, body->transform.position.z));

    }

    void gravityOn(float deltaTime) {
        body->transform.position += glm::vec3(0.0f, gravity * deltaTime, 0.0f);
        collider.setCenter(glm::vec3(body->transform.position.x, body->transform.position.y, body->transform.position.z));
    }

    void checkCollision(std::vector<BoxCollider> other) {

        for (auto i : other) {

            body->transform.position = glm::translate(glm::mat4(1.0f), glm::vec3(collider.isCollision(&i, true))) *
                glm::vec4(body->transform.position,1.0f);

        }
    }

    void detectCollision(std::vector<BoxCollider> other) {

        for (auto i : other) {

            if (collider.isCollision(&i, false).w == 1) {
                
                i.parent->getComponent<AI>(ComponentType::AI)->onTriggerEnter(i);
                //printf("trigger position: %f %f %f", i.getCenter().x, i.getCenter().y, i.getCenter().z);

            }

        }

    }

    Model *getBody() const {
        return body;
    }

    int getDirection() const {
        return front_or_left;
    }

    //zatrzymywanie podczas obrotu i ustawianie gracza przodem do kamery
    void rotate(float angle) {
        x = 0.0f;
        z = 0.0f;
//        body->transform.y_rotation_angle = angle; //zle
    }
//
    void correctAngle(int dir) {
        if(fmodf(body->transform.y_rotation_angle, 90.0f) != 0) {
            body->transform.y_rotation_angle = -90.0f * (float)dir;
        }
    }

    //???? źle
//    void rotateLeftRight(int dir) {
//        body->transform.y_rotation_angle = 90.0f * (dir + (float)dirLR) * direction;
//        if(dir == 2)
//            body->transform.y_rotation_angle = -90.0f * (dir + (float)dirLR) * direction;
//
//    }

    SphereCollider collider;

    //DEBUG
    float getVelX() {
        return x;
    }

    float getVelZ() {
        return z;
    }

    float getAngle() {
        return body->transform.y_rotation_angle;
    }

private:
    Model* body;

    float speed = 200.0f;
    int front_or_left = 1;
    float gravity = -30.0f;
    float x = 0.0f, z = 0.0f;
    int dirLR = 0;

};

#endif //DEEPER_PLAYER_H
