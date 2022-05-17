//
// Created by fujitsu on 29.04.2022.
//

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


class Player {
public:


    Player(char *path) {
        body = new Model(path);
        collider.setRadius(20.0f);
    }

    void render(Shader &shader) {
        body->Draw(shader);
    }

    //prawdopodobnie do optymalizacji
    void move(GLFWwindow* window, Camera_Facing_Direction dir, float deltaTime, float &depthPos) {
        if(dir == FRONT_DIR || dir == LEFT_DIR) {
            direction = -1;
        }
        else {
            direction = 1;
        }

        float velocity = (float)direction * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            if (dir == FRONT_DIR || dir == BACK_DIR) {
                body->transform.position += glm::vec3(
                        speed * velocity, 0.0f, 0.0f);
            } else {
                body->transform.position += glm::vec3(
                        0.0f, 0.0f, speed * velocity);
            }
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            if (dir == FRONT_DIR || dir == BACK_DIR) {
                body->transform.position += glm::vec3(
                        -speed * velocity, 0.0f, 0.0f);
            } else {
                body->transform.position += glm::vec3(
                        0.0f, 0.0f, -speed * velocity);
            }
        }


        //DEBUG
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            if (dir == FRONT_DIR || dir == BACK_DIR) {
                body->transform.position += glm::vec3(
                        0.0f, 0.0f, speed * velocity);
                    depthPos = body->transform.position.z * direction;

            } else {
                body->transform.position += glm::vec3(
                        -speed * velocity, 0.0f, 0.0f);
                depthPos = body->transform.position.x * -direction;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            if (dir == FRONT_DIR || dir == BACK_DIR) {

                body->transform.position += glm::vec3(
                        0.0f, 0.0f, -speed * velocity);

                depthPos = body->transform.position.z * direction;

            } else {
                body->transform.position += glm::vec3(
                        speed * velocity, 0.0f, 0.0f);

                depthPos = body->transform.position.x * -direction;
            }
        }

        collider.setCenter(glm::vec3(body->transform.position.x, body->transform.position.y, body->transform.position.z));

    }

    void checkCollision(std::vector<BoxCollider> other) {

        for (auto i : other) {

            //printf("%f %f %f %f %f %f\n", i.getCenter().x, i.getCenter().y, i.getCenter().z,
                //i.getSizeX(), i.getSizeY(), i.getSizeZ());
            body->transform.position = glm::translate(glm::mat4(1.0f), glm::vec3(collider.isCollision(&i, true))) *
                glm::vec4(body->transform.position,1.0f);

        }
        
        //printf("%d\n", other.size());
    }

    Model *getBody() const {
        return body;
    }

    int getDirection() const {
        return direction;
    }

    SphereCollider collider;

private:
    Model* body;

    float speed = 300.0f;
    int direction = 1;

};

#endif //DEEPER_PLAYER_H
