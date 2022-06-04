#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "hudRect.h"

enum State {
    MAIN_MENU,
    GAME_RUNNING,
    PAUSE,
    GAME_OVER,
//    RANKING,
    CREDITS,
    EXIT
};

class GameState {

public:

    explicit GameState(State curState);

    void setState(State state);
    State getCurState() const;

    void mainMenu(GLFWwindow * window, Shader shader);
    void loadMainMenuResources(int display_w, int display_h, Shader shader);

    void gameRunning(GLFWwindow * window);

    void pause(GLFWwindow * window, Shader shader);
    void loadPauseResources(int display_w, int display_h, Shader shader);

    void credits(GLFWwindow * window, Shader shader);
    void loadCreditsResources(int display_w, int display_h, Shader shader);

//    void leaderboard(GLFWwindow * window, Shader shader);
//    void loadLeaderboardResources(int display_w, int display_h, Shader shader);

    void gameOver(GLFWwindow * window, Shader shader);
    void loadGameOverResources(int display_w, int display_h, Shader shader);

    void processMouse(float posX, float posY);
    void processMouseButton(bool action);

    void setSetUp(bool setUp);

private:
    State curState;
    std::vector<hudRect> btns;
    std::vector<hudRect> elements;

    //zmienne pomocnicze
    bool setUp = true;
    int btnHover = -1;
    bool btnPress = false;
    bool moveHighlight = true;

    bool checkCollision(float posX, float posY, hudRect btn);

};
