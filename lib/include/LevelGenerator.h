#include <utility>
#include <algorithm>
#include <random>

#ifndef DEEPER_LEVELGENERATOR_H
#define DEEPER_LEVELGENERATOR_H

//TODO: skala & gameObject
class LevelGenerator {

public:
    LevelGenerator(std::vector<Model>& _caveModels) {
        caveModels = _caveModels;
    }

    void newGame(int height) {
        randomize();
        initPos = (float)height * -1.0f - 800.0f; //TODO: dopasowac

        //gora
        caveModels[num[0]].transform.position = glm::vec3(0.0f, initPos + levelH, 0.0f);
        caveModels[num[0]].transform.y_rotation_angle = 90.0f * randNum();

        //srodek
        caveModels[num[1]].transform.position = glm::vec3(0.0f, initPos, 0.0f);
        caveModels[num[1]].transform.y_rotation_angle = 90.0f * randNum();

        //dol
        caveModels[num[2]].transform.position = glm::vec3(0.0f, initPos - levelH, 0.0f);
        caveModels[num[2]].transform.y_rotation_angle = 90.0f * randNum();
    }

    void DrawLevels(Shader shader) {
        for(auto& cave : caveModels) {
            cave.Draw(shader);
        }

//        caveModels[num[1]].Draw(shader);
    }

    void move(float pos) {
        caveModels[num[1]].transform.position = glm::vec3(0.0f, pos, 0.0f);
    }

    void update(float camY) {
        if(camY < levelH/2.0f - levelH * (iter + 1)) { //TODO: dopasowac
            caveModels[num[iter % 3]].transform.position = glm::vec3(0.0f, initPos - levelH * (float)(iter + 2), 0.0f);
            caveModels[num[iter % 3]].transform.y_rotation_angle = 90.0f * randNum();
            iter++;
        }
    }

    const int *getNum() const {
        return num;
    }


private:
    float initPos;
    std::vector<Model> caveModels;
    int num[3] = {0, 1, 2};
    float levelH = 1810.0f;
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
