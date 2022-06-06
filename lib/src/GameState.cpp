#include <iostream>
#include "GameState.h"

GameState::GameState(State curState) : curState(curState) {}

void GameState::setState(State state) {
    curState = state;
    setUp = true;
}

State GameState::getCurState() const {
    return curState;
}

///GAME STATES
void GameState::mainMenu(GLFWwindow * window, Shader shader) {

    if(setUp) {
        btnPress = false;
        glDepthMask(GL_FALSE);
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        loadMainMenuResources(display_w, display_h, shader);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        setUp = false;
    }

    for(int i = 0; i < elements.size() - 1; i++) {
        elements[i].Draw(shader);
    }

    if(btnHover != -1) {
        if(btnPress) {
            setState(static_cast<State>(btns[btnHover].getState()));
        }
        if(moveHighlight) {
            elements.back().setCoords(elements.back().w, elements.back().h, elements.back().x, btns[btnHover].y - btns[btnHover].h/2.0f - 5.0f);
            moveHighlight = false;
        }
        elements.back().Draw(shader);
    }
    else
        moveHighlight = true;
}

void GameState::gameRunning(GLFWwindow *window) {
    if (setUp) {
        btnPress = false;
//        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        setUp = false;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        setState(PAUSE);
    }
}

void GameState::pause(GLFWwindow *window, Shader shader) {
    if(setUp) {
        btnPress = false;
        glDepthMask(GL_FALSE);
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        loadPauseResources(display_w, display_h, shader);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        setUp = false;
    }

    for(int i = 0; i < elements.size() - 1; i++) {
        elements[i].Draw(shader);
    }

    if(btnHover != -1) {
        if(btnPress) {
            setState(static_cast<State>(btns[btnHover].getState()));
        }
        if(moveHighlight) {
            elements.back().setCoords(elements.back().w, elements.back().h, elements.back().x, btns[btnHover].y - btns[btnHover].h/2.0f - 5.0f);
            moveHighlight = false;
        }
        elements.back().Draw(shader);
    }
    else
        moveHighlight = true;
}

void GameState::credits(GLFWwindow *window, Shader shader) {
    if(setUp) {
        btnPress = false;
        glDepthMask(GL_FALSE);
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        loadCreditsResources(display_w, display_h, shader);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        setUp = false;
    }

    for(int i = 0; i < elements.size() - 1; i++) {
        elements[i].Draw(shader);
    }

    if(btnHover != -1) {
        if(btnPress) {
            setState(static_cast<State>(btns[btnHover].getState()));
        }
        if(moveHighlight) {
            elements.back().setCoords(elements.back().w, elements.back().h, elements.back().x, btns[btnHover].y - btns[btnHover].h/2.0f - 5.0f);
            moveHighlight = false;
        }
        elements.back().Draw(shader);
    }
    else
        moveHighlight = true;
}

void GameState::gameOver(GLFWwindow *window, Shader shader) {
    if(setUp) {
        btnPress = false;
        glDepthMask(GL_FALSE);
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        loadGameOverResources(display_w, display_h, shader);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        setUp = false;
    }

    for(int i = 0; i < elements.size() - 1; i++) {
        elements[i].Draw(shader);
    }

    if(btnHover != -1) {
        if(btnPress) {
            setState(static_cast<State>(btns[btnHover].getState()));
        }
        if(moveHighlight) {
            elements.back().setCoords(elements.back().w, elements.back().h, elements.back().x, btns[btnHover].y - btns[btnHover].h/2.0f - 5.0f);
            moveHighlight = false;
        }
        elements.back().Draw(shader);
    }
    else
        moveHighlight = true;
}

///RESOURCES
void GameState::loadMainMenuResources(int display_w, int display_h, Shader shader) {
    elements.clear();
    btns.clear();

    hudRect background((float)display_w, (float)display_h, 0.0f, 0.0f,
                       "./res/hud/main_menu.png", shader);
    hudRect start(212, 72, (float)(display_w - 212)/2.0f, 600.0f / 1080.0f * (float)display_h,
                  "./res/hud/start.png", shader);
    hudRect creds(296, 72, (float)(display_w - 296)/2.0f, 443.0f / 1080.0f * (float)display_h,
                  "./res/hud/credits.png", shader);
    hudRect exit(170, 72, (float)(display_w - 170)/2.0f, 288.0f / 1080.0f * (float)display_h,
                 "./res/hud/exit.png", shader);
    hudRect highlight(658, 145, (float)(display_w - 658)/2.0f, -300.0f,
                      "./res/hud/highlight.png", shader);

    start.setState((int)GAME_RUNNING);
    creds.setState((int)CREDITS);
    exit.setState((int)EXIT);

    elements = {background, start, creds, exit, highlight};
    btns = {start, creds, exit};

}

void GameState::loadPauseResources(int display_w, int display_h, Shader shader) {
    elements.clear();
    btns.clear();

    hudRect pauseBackground(700, 564, (display_w - 700)/2.0f, (display_h - 564)/2.0f,
                            "./res/hud/pause.png", shader);
    hudRect resume(254, 72, (display_w - 254)/2.0f, 540.0f / 1080.0f * (float)display_h,
                   "./res/hud/resume.png", shader);
    hudRect mainMenu(382, 72, (display_w - 382)/2.0f, 400.0f / 1080.0f * (float)display_h,
                     "./res/hud/menu_back.png", shader);
    hudRect highlight(658, 145, (display_w - 658)/2.0f, -300.0f,
                      "./res/hud/highlight.png", shader);

    resume.setState((int)GAME_RUNNING);
    mainMenu.setState((int)MAIN_MENU);

    elements = {pauseBackground, resume, mainMenu, highlight};
    btns = {resume, mainMenu};
}

void GameState::loadCreditsResources(int display_w, int display_h, Shader shader) {
    elements.clear();
    btns.clear();

    hudRect creditsBackground(display_w, display_h, 0.0f, 0.0f,
                              "./res/hud/credits_background.png", shader);
    hudRect mainMenu(382, 72, (display_w - 382)/2.0f, 100.0f / 1080.0f * (float)display_h,
                     "./res/hud/menu_back.png", shader);
    hudRect highlight(658, 145, (display_w - 658)/2.0f, -300.0f,
                      "./res/hud/highlight.png", shader);

    mainMenu.setState((int)MAIN_MENU);

    elements = {creditsBackground, mainMenu, highlight};
    btns = {mainMenu};

}

//void GameState::loadLeaderboardResources(int display_w, int display_h, Shader shader) {
//    elements.clear();
//    btns.clear();
//
//    hudRect leaderboardBackground(display_w, display_h, 0.0f, 0.0f,
//                              "./res/hud/leaderboard_background.png", shader);
//    hudRect mainMenu(382, 72, (display_w - 382)/2.0f, 100.0f / 1080.0f * (float)display_h,
//                     "./res/hud/menu_back.png", shader);
//    hudRect highlight(658, 145, (display_w - 658)/2.0f, -300.0f,
//                      "./res/hud/highlight.png", shader);
//
//    mainMenu.setState((int)MAIN_MENU);
//
//    elements = {leaderboardBackground, mainMenu, highlight};
//    btns = {mainMenu};
//}

void GameState::loadGameOverResources(int display_w, int display_h, Shader shader) {
    elements.clear();
    btns.clear();

    hudRect gameOverBackground(display_w, display_h, 0.0f, 0.0f,
                               "./res/hud/game_over.png", shader);
    hudRect score(182, 62, (display_w - 182)/2.0f, 650.0f / 1080.0f * (float)display_h,
                  "./res/hud/score.png", shader);
    hudRect restart(296, 72, (display_w - 296)/2.0f, 350.0f / 1080.0f * (float)display_h,
                    "./res/hud/restart.png", shader);
    hudRect mainMenu(382, 72, (display_w - 382)/2.0f, 200.0f / 1080.0f * (float)display_h,
                     "./res/hud/menu_back.png", shader);
    hudRect highlight(658, 145, (display_w - 658)/2.0f, -300.0f,
                      "./res/hud/highlight.png", shader);

    restart.setState((int)GAME_RUNNING);
    mainMenu.setState((int)MAIN_MENU);

    elements = {gameOverBackground, score, restart, mainMenu, highlight};
    btns = {restart, mainMenu};
}

///HELPERS
void GameState::processMouse(float posX, float posY) {
    for(int i = 0; i < btns.size(); i++) {
        if(checkCollision(posX, posY, btns[i])) {
            btnHover = i;
            break;
        }
        btnHover = -1;
    }
}

bool GameState::checkCollision(float posX, float posY, hudRect btn) {
    if(posX > btn.x && posX < btn.x + btn.w && posY > btn.y && posY < btn.y + btn.h)
        return true;
    else
        return false;
}

void GameState::processMouseButton(bool action) {
    btnPress = action;
}

void GameState::setSetUp(bool setUp) {
    GameState::setUp = setUp;
}