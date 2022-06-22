#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "hudRect.h"

class Tutorial {
public:

    Tutorial(int display_w, int display_h, Shader shader) {
        loadResources(display_w, display_h, shader);
    }

    //TODO: rotate, pickThrow, pickGrab mniejsze o 260/315
    void loadResources(int display_w, int display_h, Shader shader) {
        info.clear();
        hudRect move(260, 211,
                     display_w/2.0f - 260.0f - 50.0f, -display_h/2.0f + 211.0f/2.0f - 50.0f,
                     "./res/tutorial/move.png", shader);
        hudRect rotate(364 * 260/315, 318 * 260/315,
                       display_w/2.0f - 364.0f * 260/315 - 50.0f, -display_h/2.0f + 318.0f/2.0f * 260/315 - 50.0f,
                       "./res/tutorial/rotate.png", shader);
        hudRect pickThrow(400 * 260/315, 340 * 260/315,
                          display_w/2.0f - 400.0f * 260/315 - 50.0f, -display_h/2.0f + 340.0f/2.0f * 260/315 - 50.0f,
                          "./res/tutorial/throw.png", shader);
        hudRect pickGrab(400 * 260/315, 340 * 260/315,
                         display_w/2.0f - 400.0f * 260/315 - 50.0f, -display_h/2.0f + 340.0f/2.0f * 260/315 - 50.0f,
                         "./res/tutorial/retrieve.png", shader);
        hudRect jump(286, 255,
                     display_w/2.0f - 285.0f - 50.0f, -display_h/2.0f + 255.0f/2.0f - 50.0f,
                     "./res/tutorial/jump.png", shader);

        info = {move, jump, rotate, pickThrow, pickGrab};
    }

    void display(GLFWwindow * window, Shader shader) {
        if(setUp) {
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            loadResources(display_w, display_h, shader);
            setUp = false;
        }

        if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && n == 0)
            n++;
        else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && n == 1)
            n++;
        else if((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && n == 2)
            n++;
        else if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && n == 3)
            n++;
        else if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && n == 4)
            n++;

        if(n < 5)
            info[n].Draw(shader);
    }

    void setSetUp(bool setUp) {
        Tutorial::setUp = setUp;
    }

    void newGame() {
        n = 0;
    }

    int getN() const {
        return n;
    }


private:

    int n = 0;
    std::vector<hudRect> info;

    bool setUp = true;


};
