#include "imgui.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "GameObject.h"
#include "CubeMesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Shader.h"
#include "SphereMesh.h"
#include "SceneGraphNode.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "typedefs.h"

#include <stdio.h>
#include <memory>
#include <algorithm>
#include <iterator>
#include <map>

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually. 
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
void useDebugCamera(glm::mat4 &proj, glm::mat4 &view, GLFWwindow * window, float &scale);
void useOrthoCamera(glm::mat4 &proj, glm::mat4 &view, GLFWwindow * window, float &cameraY, float totalTime, float &scale);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//DEBUG
float testScale = 1;
float testScale2 = 1;
float xPos = 1;
float yPos = 1;
bool collision = false;

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Deeper", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    FMOD_RESULT result; //TODO: SoundManager
    FMOD::System* system = NULL;

    result = FMOD::System_Create(&system);      // Create the main system object.
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return(-1);
    }

    result = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return(-1);
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();

    bool show_demo_window = false;
    bool show_camera_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    nodePtr root(new SceneGraphNode());
    nodePtr cube1(new SceneGraphNode());
    nodePtr sphere1(new SceneGraphNode());

    gameObjectPtr cube(new GameObject());
    gameObjectPtr sphere(new GameObject());

    cube1->addGameObject(cube);
    sphere1->addGameObject(sphere);
    cube1->addChild(sphere1);

    cube->addComponent<CubeMesh>();
    cube->addComponent<Transform>();
    cube->addComponent<BoxCollider>();
    cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(glm::vec3(0.0f));
    cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(1.0f));

    sphere->addComponent<SphereMesh>();
    sphere->addComponent<Transform>();
    sphere->addComponent<SphereCollider>();
    sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(glm::vec3(1.0f));
    sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setRadius(1.0f);

    //TODO: funckja w klasie SceneGraphNode (wymog transforma)
    sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(1.0f, 1.0f, 1.0f);
    //do zakomentowania jesli chcemy uzyc tej drugiej kamery
    //cube2->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position += 100.0f;

    //TODO: tymczasowe, trzeba to rozdzielic w odpowiednich klasach
    Shader basicShader("./res/shaders/basic.vert", "./res/shaders/basic.frag");
    Shader textShader("./res/shaders/text.vert", "./res/shaders/text.frag");

    basicShader.use();
 
    unsigned int VAO2, VBO2 = sphere->getComponent<SphereMesh>(ComponentType::SPHEREMESH)->getVBO();

    glGenBuffers(1, &VBO2);
    glGenVertexArrays(1, &VAO2);

    std::vector<float> vertices = cube->getComponent<CubeMesh>(ComponentType::CUBEMESH)->getVertices();
    std::vector<float> vertices2 = sphere->getComponent<SphereMesh>(ComponentType::SPHEREMESH)->getVertices();

    //DEBUG
    float pVertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices2.size(), &vertices2.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    camera.SetProjMatrix(SCR_WIDTH, SCR_HEIGHT, -100.0f, 100.0f);

    float totalTime = 0.0f; //used for continuous rotation
    bool debugCamera = true, falling = false; //ImGui variables but useless without visible cursor
    float cameraY = 0.0f;
    float scale = 1.0f;


    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        totalTime += deltaTime;

        cube1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(testScale);
        sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(testScale2);
        sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(xPos, yPos, 1.0f);
        sphere1->getGameObject()->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(glm::vec3(xPos, yPos, 1.0f));
        cube1->updateTransform();
        cube1->update(nullptr, false);


        collision = false;

        //TODO: w klasie colliderow
        {   
            BoxCollider* cubeCollider = cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER);
            SphereCollider* sphereCollider = sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER);
            float x = glm::max(cubeCollider->getMinX(), glm::min(sphereCollider->getCenter().x, cubeCollider->getMaxX()));
            float y = glm::max(cubeCollider->getMinY(), glm::min(sphereCollider->getCenter().y, cubeCollider->getMaxY()));
            float z = glm::max(cubeCollider->getMinZ(), glm::min(sphereCollider->getCenter().z, cubeCollider->getMaxZ()));

            // this is the same as isPointInsideSphere
            float distance = glm::sqrt((x - sphereCollider->getCenter().x) * (x - sphereCollider->getCenter().x) +
                                       (y - sphereCollider->getCenter().y) * (y - sphereCollider->getCenter().y) +
                                       (z - sphereCollider->getCenter().y) * (z - sphereCollider->getCenter().y));

            collision = distance < sphereCollider->getRadius();
        }

        if (collision) printf("COLLISION");

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::NewLine();
            ImGui::Checkbox("Camera debug", &show_camera_window);

            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_camera_window)
        {
            ImGui::Begin("Another Window", &show_camera_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_camera_window = false;
            ImGui::End();
        }
        
        int display_w, display_h;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);

        useDebugCamera(proj, view, window, scale);
        //useOrthoCamera(proj, view, window, cameraY, totalTime, scale);

        //TODO: renderManager/meshComponent
        basicShader.use();

        basicShader.setFloat("scale", scale);
        basicShader.setMat4("projection", proj);
        basicShader.setMat4("view", view);

        glm::mat4 model = cube1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->getCombinedMatrix();
        basicShader.setMat4("model", model);

        glBindVertexArray(cube1->getGameObject()->getComponent<CubeMesh>(ComponentType::CUBEMESH)->getVAO());
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->worldMatrix;
        basicShader.setMat4("model", model);

        glBindVertexArray(VAO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices2.size(), &vertices2.front(), GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, vertices2.size()/3);

        ImGui::Render();
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void useDebugCamera(glm::mat4 &proj, glm::mat4 &view, GLFWwindow * window, float &scale) {
    scale = 1.0f;
    processInput(window);
    proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
}

void useOrthoCamera(glm::mat4 &proj, glm::mat4 &view, GLFWwindow * window, float &cameraY, float totalTime, float &scale) {

    scale = 100.0f;
    cameraY -= 0.2f;
    //camera.ContinousMovement(totalTime, 6.0f, cameraY);
    proj = camera.GetProjMatrix();
    view = camera.GetOrthoViewMatrix();

    glfwSetKeyCallback(window, key_callback);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    //DEBUG
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        testScale+=0.1;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        testScale-=0.1;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        testScale2+=0.1;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        testScale2-=0.1;

    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        yPos += 0.1;
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        yPos -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        xPos += 0.1;
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        xPos -= 0.1;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//zeby bylo pojedyncze wywolanie
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        std::cout << "Right arrow pressed\n";
        camera.ProcessMovement(6.0f, 1.0f);
    }

    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        std::cout << "Left arrow pressed\n";
        camera.ProcessMovement(6.0f, -1.0f);
    }
}