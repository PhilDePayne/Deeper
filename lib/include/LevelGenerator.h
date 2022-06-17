#include <utility>
#include <algorithm>
#include <random>

#include "LogMacros.h"

#ifndef DEEPER_LEVELGENERATOR_H
#define DEEPER_LEVELGENERATOR_H

//TODO: lamps, spawners, jakies bardziej eleganckie wykonanie bo lala co to ma byc wgl
class LevelGenerator {

public:
    LevelGenerator(std::vector<Model> _caveModels, std::vector<std::vector<Model>> _walls, std::vector<Model> _floors,
                   std::vector<Model> _lampModels, std::vector<gameObjectPtr> _lampObjects, std::vector<Model> _lampColliders) {
        caveModels = std::move(_caveModels);
        walls = std::move(_walls);
        floors = std::move(_floors);
        lampModels = std::move(_lampModels);
        lampObjects = std::move(_lampObjects);
        lampColliders = std::move(_lampColliders);
    }

    void newGame(int height) {
        randomize();
        initPos = (float)height * -1.0f - 800.0f;

        //gora
        int r = randNum();
        caveModels[num[0]].transform.position = glm::vec3(0.0f, initPos + levelH - 10.0f, 0.0f);
//        caveModels[num[0]].transform.y_rotation_angle = 90.0f * r;

        walls[num[0]][0].transform.position = glm::vec3(0.0f, initPos + levelH, 0.0f);
//        walls[num[0]][0].transform.y_rotation_angle = 90.0f * r;
        walls[num[0]][1].transform.position = glm::vec3(0.0f, initPos + levelH, 0.0f);
//        walls[num[0]][1].transform.y_rotation_angle = 90.0f * r;

        floors[num[0]].transform.position = glm::vec3(0.0f, initPos + levelH, 0.0f);
//        floors[num[0]].transform.y_rotation_angle = 90.0f * r;

        lampModels[num[0]].transform.position = glm::vec3(0.0f, initPos + levelH - 10.0f, 0.0f);
        lampObjects[num[0]]->getComponent<Model>(ComponentType::MODEL)->transform.position = glm::vec3(0.0f, initPos + levelH, 0.0f);

        //srodek
        r = randNum();
        caveModels[num[1]].transform.position = glm::vec3(0.0f, initPos - 10.0f, 0.0f);
//        caveModels[num[1]].transform.y_rotation_angle = 90.0f * r;

        walls[num[1]][0].transform.position = glm::vec3(0.0f, initPos, 0.0f);
//        walls[num[1]][0].transform.y_rotation_angle = 90.0f * r;
        walls[num[1]][1].transform.position = glm::vec3(0.0f, initPos, 0.0f);
//        walls[num[1]][1].transform.y_rotation_angle = 90.0f * r;

        floors[num[1]].transform.position = glm::vec3(0.0f, initPos, 0.0f);
//        floors[num[1]].transform.y_rotation_angle = 90.0f * r;

        lampModels[num[1]].transform.position = glm::vec3(0.0f, initPos - 10.0f, 0.0f);
        lampObjects[num[1]]->getComponent<Model>(ComponentType::MODEL)->transform.position = glm::vec3(0.0f, initPos, 0.0f);

        lampColliders[num[1]].transform.position = glm::vec3(0.0f, initPos, 0.0f);

        //dol
        r = randNum();
        caveModels[num[2]].transform.position = glm::vec3(0.0f, initPos - levelH - 10.0f, 0.0f);
//        caveModels[num[2]].transform.y_rotation_angle = 90.0f * r;

        walls[num[2]][0].transform.position = glm::vec3(0.0f, initPos - levelH, 0.0f);
//        walls[num[2]][0].transform.y_rotation_angle = 90.0f * r;
        walls[num[2]][1].transform.position = glm::vec3(0.0f, initPos - levelH, 0.0f);
//        walls[num[2]][1].transform.y_rotation_angle = 90.0f * r;

        floors[num[2]].transform.position = glm::vec3(0.0f, initPos - levelH, 0.0f);
//        floors[num[2]].transform.y_rotation_angle = 90.0f * r;

        lampModels[num[2]].transform.position = glm::vec3(0.0f, initPos - levelH - 10.0f, 0.0f);
        lampObjects[num[2]]->getComponent<Model>(ComponentType::MODEL)->transform.position = glm::vec3(0.0f, initPos - levelH, 0.0f);

        cur = num[1];

#ifdef DEEPER_LEVELGENERATOR_LOGS
        std::cout << "\n" << num[0] << " " << num[1] << " " << num[2] << "\ncur " << cur;
#endif
    }

    void DrawLevels(Shader shader) {
//        for(auto& cave : caveModels) {
//            cave.Draw(shader);
//        }

        for(int i = 0; i < 3; i++) {
            caveModels[i].Draw(shader);
            lampModels[i].Draw(shader);
        }

        lampObjects[cur]->getComponent<Model>(ComponentType::MODEL)->Draw(shader);

    }

    void move(float pos) {
        caveModels[num[1]].transform.position = glm::vec3(0.0f, pos, 0.0f);
    }

    void update(float camY) {
        if(camY < levelH/2.0f - levelH * (iter + 1)) {
            int r = randNum();
            float newPos = initPos - levelH * (float)(iter + 2);

            caveModels[num[iter % 3]].transform.position = glm::vec3(0.0f, newPos - 10.0f, 0.0f);
//            caveModels[num[iter % 3]].transform.y_rotation_angle = 90.0f * r;

            walls[num[iter % 3]][0].transform.position = glm::vec3(0.0f, newPos, 0.0f);
//            walls[num[iter % 3]][0].transform.y_rotation_angle = 90.0f * r;
            walls[num[iter % 3]][1].transform.position = glm::vec3(0.0f, newPos, 0.0f);
//            walls[num[iter % 3]][1].transform.y_rotation_angle = 90.0f * r;

            floors[num[iter % 3]].transform.position = glm::vec3(0.0f, newPos, 0.0f);
//            floors[num[iter % 3]].transform.y_rotation_angle = 90.0f * r;

            lampModels[num[iter % 3]].transform.position = glm::vec3(0.0f, newPos - 10.0f, 0.0f);
            lampObjects[num[iter % 3]]->getComponent<Model>(ComponentType::MODEL)->transform.position = glm::vec3(0.0f, newPos - 10.0f, 0.0f);

            std::cout << "\nnew " << num[iter % 3];

            iter++;
        }
    }

    const int *getNum() const {
        return num;
    }

    //TODO: void w main loop
    int whereAmI(float posY) {
        if(posY < -levelH  * iter) {
            cur = num[(iter + 1) % 3];
        }
        return cur;
    }

    void collisions(Player* player) {
        player->checkCollision(walls[cur][0].getColliders());
        player->checkCollision(walls[cur][1].getColliders());
        player->checkCollision(floors[cur].getColliders());
    }

    void triggers(Player* player) {
//        player->detectCollision(lampObjects[cur]->getComponent<Model>(ComponentType::MODEL)->getColliders());
        player->detectCollision(lampColliders[cur].getColliders());
    }


private:
    std::vector<Model> caveModels;
    std::vector<Model> floors;
    std::vector<std::vector<Model>> walls;
    std::vector<Model> lampModels;
    std::vector<gameObjectPtr> lampObjects;
    std::vector<Model> lampColliders;

    float initPos;
    int num[3] = {0, 1, 2};
    float levelH = 1900.0f;
    int iter = 0;
    int cur;


    void randomize() {
        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(std::begin(num), std::end(num), g);
    }

    int randNum() {
        std::random_device generator;
        std::mt19937 mt(generator());
        std::uniform_int_distribution<int> distribution(0,3);
        return distribution(mt);
    }


};

#endif //DEEPER_LEVELGENERATOR_H
