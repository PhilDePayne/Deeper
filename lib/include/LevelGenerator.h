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
                   std::vector<Model> _lampModels, std::vector<Model> _lampColliders, std::vector<Model> _spawnerColliders) {
        caveModels = std::move(_caveModels);
        walls = std::move(_walls);
        floors = std::move(_floors);
        lampModels = std::move(_lampModels);
        lampColliders = std::move(_lampColliders);
        spawnerColliders = std::move(_spawnerColliders);
    }

    void newGame(int height) {
        randomize();
        initPos = (float)height * -1.0f - 800.0f;
        float pos[3] = {initPos + levelH, initPos, initPos - levelH}; //gora - srodek - dol (gracz zaczyna na gorze srodka)

        //positions
        for(int i = 0; i < 3; i++) {
            caveModels[num[i]].transform.position = glm::vec3(0.0f, pos[i] - 10.0f, 0.0f);

            walls[num[i]][0].transform.position = glm::vec3(0.0f, pos[i], 0.0f);
            walls[num[i]][1].transform.position = glm::vec3(0.0f, pos[i], 0.0f);

            floors[num[i]].transform.position = glm::vec3(0.0f, pos[i], 0.0f);

            lampModels[num[i]].transform.position = glm::vec3(0.0f, pos[i] - 10.0f, 0.0f);
            lampColliders[num[i]].transform.position = glm::vec3(0.0f, pos[i], 0.0f);

            spawnerColliders[num[i]].transform.position = glm::vec3(0.0f, pos[i], 0.0f);
        }

        //rotation
        for(int i : num) {
            int r = randNum();
            caveModels[i].transform.y_rotation_angle = 90.0f * r;
            walls[i][0].transform.y_rotation_angle = 90.0f * r;
            walls[i][1].transform.y_rotation_angle = 90.0f * r;
            floors[i].transform.y_rotation_angle = 90.0f * r;
            lampModels[i].transform.y_rotation_angle = 90.0f * r;
            lampColliders[i].transform.y_rotation_angle = 90.0f * r;
            spawnerColliders[i].transform.y_rotation_angle = 90.0f * r;
        }

        cur = num[1];

#ifdef DEEPER_LEVELGENERATOR_LOGS
        std::cout << "\n" << num[0] << " " << num[1] << " " << num[2] << "\ncur " << cur << "\n";
#endif
    }

    void DrawLevels(Shader shader) {
        for(auto& cave : caveModels) {
            cave.Draw(shader);
        }

//        for(int i = 0; i < 3; i++) {
//            caveModels[i].Draw(shader);
//            lampModels[i].Draw(shader);
//        }

        lampModels[cur].Draw(shader);
//        floors[cur].Draw(shader);

    }

    void update(float camY) {
        if(camY < levelH/2.0f - levelH * (iter + 1)) {
            int r = randNum();
            float newPos = initPos - levelH * (float)(iter + 2);

            caveModels[num[iter % 3]].transform.position = glm::vec3(0.0f, newPos - 10.0f, 0.0f);
            caveModels[num[iter % 3]].transform.y_rotation_angle = 90.0f * r;

            walls[num[iter % 3]][0].transform.position = glm::vec3(0.0f, newPos, 0.0f);
            walls[num[iter % 3]][0].transform.y_rotation_angle = 90.0f * r;
            walls[num[iter % 3]][1].transform.position = glm::vec3(0.0f, newPos, 0.0f);
            walls[num[iter % 3]][1].transform.y_rotation_angle = 90.0f * r;

            floors[num[iter % 3]].transform.position = glm::vec3(0.0f, newPos, 0.0f);
            floors[num[iter % 3]].transform.y_rotation_angle = 90.0f * r;

            lampModels[num[iter % 3]].transform.position = glm::vec3(0.0f, newPos - 10.0f, 0.0f);
            lampModels[num[iter % 3]].transform.y_rotation_angle = 90.0f * r;
            lampColliders[num[iter % 3]].transform.position = glm::vec3(0.0f, newPos, 0.0f);
            lampColliders[num[iter % 3]].transform.y_rotation_angle = 90.0f * r;

            spawnerColliders[num[iter % 3]].transform.position = glm::vec3(0.0f, newPos, 0.0f);
            spawnerColliders[num[iter % 3]].transform.y_rotation_angle = 90.0f * r;

#ifdef DEEPER_LEVELGENERATOR_LOGS
            std::cout << "\nnew cave: " << num[iter % 3];
#endif

            iter++;
        }
    }


    void whereAmI(float posY) {
        if(posY < -levelH  * iter) {
            cur = num[(iter + 1) % 3];
        }
    }

    void collisions(Player* player) {
        player->checkCollision(walls[cur][0].getColliders());
        player->checkCollision(walls[cur][1].getColliders());
        player->checkCollision(floors[cur].getColliders());
    }

    void triggers(Player* player) {
        player->detectCollision(lampColliders[cur].getColliders());
        player->detectCollision(spawnerColliders[cur].getColliders());
    }

    Model* getCurrentFloor() {
        return &floors[cur];
    }

    Model* getCurrentLamps() {
        return &lampColliders[cur];
    }

    int getCur() {
        return cur;
    }


private:
    std::vector<Model> caveModels;
    std::vector<Model> floors;
    std::vector<std::vector<Model>> walls;
    std::vector<Model> lampModels;
    std::vector<Model> lampColliders;
    std::vector<Model> spawnerColliders;

    float initPos;
    int num[3] = {0, 1, 2};
    float levelH = 1850.0f;
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
