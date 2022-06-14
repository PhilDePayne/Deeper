#include <utility>
#include <algorithm>
#include <random>

#ifndef DEEPER_LEVELGENERATOR_H
#define DEEPER_LEVELGENERATOR_H

//TODO: skala & gameObject
class LevelGenerator {

public:
    LevelGenerator(gameObjectPtr cave1, gameObjectPtr cave2, gameObjectPtr cave3) {
        caves = {std::move(cave1), std::move(cave2), std::move(cave3)};
    }

    void newGame(int height) {
        randomize();
        initPos = (float)height * -1.0f - 300.0f; //TODO: dopasowac

        //gora
//        caves[num[0]].transform.position = glm::vec3(0.0f, initPos + levelH, 0.0f);
//        caves[num[0]].transform.y_rotation_angle = 90.0f * randNum();
//
//        //srodek
//        caves[num[1]].transform.position = glm::vec3(0.0f, initPos, 0.0f);
//        caves[num[1]].transform.y_rotation_angle = 90.0f * randNum();
//
//        //dol
//        caves[num[2]].transform.position = glm::vec3(0.0f, initPos - levelH, 0.0f);
//        caves[num[2]].transform.y_rotation_angle = 90.0f * randNum();
    }

    void DrawLevels(Shader shader) {
        for(auto& cave : caves) {
//            cave.Draw(shader);
        }

//        caveModels[num[1]].Draw(shader);
    }

    void move(float pos) {
//        caves[num[1]].transform.position = glm::vec3(0.0f, pos, 0.0f);
    }

    void update(float camY) {
        if(camY < levelH/2.0f - levelH * (iter + 1)) { //TODO: dopasowac
//            caves[num[iter % 3]].transform.position = glm::vec3(0.0f, initPos - levelH * (float)(iter + 2), 0.0f);
//            caves[num[iter % 3]].transform.y_rotation_angle = 90.0f * randNum();
            iter++;
        }
    }

    const int *getNum() const {
        return num;
    }


private:
    float initPos;
    std::vector<gameObjectPtr> caves;
    int num[3] = {0, 1, 2};
    float levelH = 1040.0f;
    int iter = 0;


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
