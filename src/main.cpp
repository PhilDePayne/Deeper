#include "imgui.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.h"
#include "CubeMesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Shader.h"
#include "SphereMesh.h"
#include "SceneGraphNode.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "LightSource.h"
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
void useOrthoCamera(glm::mat4 &proj, glm::mat4 &view, GLFWwindow * window, float &cameraY, float &scale);

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
float xPos = 0.0;
float yPos = 1.8;
float zPos = 0.0;
bool collision = false;
bool move = 0;
glm::mat4 projection = glm::ortho(0.0f, 720.0f, 0.0f, 1280.0f);
int rotate = 0;

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
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Deeper", NULL, NULL);
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

    Shader basicShader("./res/shaders/basic.vert", "./res/shaders/basic.frag");

    nodePtr root(new SceneGraphNode());
    nodePtr cube1parent(new SceneGraphNode());
    nodePtr cube1(new SceneGraphNode());
    nodePtr cube2(new SceneGraphNode());
    nodePtr cube3parent(new SceneGraphNode());
    nodePtr cube3(new SceneGraphNode());
    nodePtr cube4(new SceneGraphNode());
    nodePtr sphere1(new SceneGraphNode());

    gameObjectPtr rootGO(new GameObject());
    gameObjectPtr cubeParent(new GameObject());
    gameObjectPtr cube(new GameObject());
    gameObjectPtr backgroundCube(new GameObject());
    gameObjectPtr rightCubeParent(new GameObject());
    gameObjectPtr rightCube(new GameObject());
    gameObjectPtr leftCube(new GameObject());
    gameObjectPtr sphere(new GameObject());
    gameObjectPtr spotLight(new GameObject());
    gameObjectPtr dirLight(new GameObject());


    //OBJECT PARAMETERS
    {
        root->addChild(cube1parent);
        root->addChild(cube2);
        root->addChild(cube3parent);
        root->addChild(sphere1);
        //root->addChild(cube1);
        //root->addChild(cube3);

        cube1parent->addChild(cube1);
        cube3parent->addChild(cube3);
        cube1parent->addChild(cube4);

        //cube1->addChild(cube2);
        //cube1->addChild(sphere1);

        cube1->addGameObject(cube);
        cube1parent->addGameObject(cubeParent);
        cube2->addGameObject(backgroundCube);
        cube3parent->addGameObject(rightCubeParent);
        cube3->addGameObject(rightCube);
        sphere1->addGameObject(sphere);
        root->addGameObject(rootGO);
        cube4->addGameObject(leftCube);

        rootGO->addComponent<Transform>();
        rootGO->getComponent<Transform>(ComponentType::TRANSFORM)->z_rotation_angle = 0.0f;

        cube->addComponent<CubeMesh>();
        cube->addComponent<Transform>();
        cube->getComponent<Transform>(ComponentType::TRANSFORM)->z_rotation_angle = 45.0f;
        cube->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(0.0f, 0.0f, 0.0f);
        cube->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(0.22f);
        cube->addComponent<BoxCollider>();
        cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(glm::vec3(-3.0f, 0.0, 0.0f));
        cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(0.22f * 3.0f));

        leftCube->addComponent<CubeMesh>();
        leftCube->addComponent<Transform>();
        leftCube->getComponent<Transform>(ComponentType::TRANSFORM)->z_rotation_angle = 0.0f;
        leftCube->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(0.0f, 0.2f, 0.0f);
        leftCube->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(0.05f);
        leftCube->addComponent<BoxCollider>();
        leftCube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(glm::vec3(0.0f - 3.0f, 0.6f, 0.0f - 0.0f));
        leftCube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(0.05f * 3.0f));

        cubeParent->addComponent<CubeMesh>();
        cubeParent->addComponent<Transform>();
        cubeParent->getComponent<Transform>(ComponentType::TRANSFORM)->z_rotation_angle = 0.0f;
        cubeParent->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(-3.0f, 0.0f, 0.0f);
        cubeParent->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(3.0f);
        cubeParent->addComponent<BoxCollider>();
        cubeParent->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(glm::vec3(-3.0f, 0.0, 0.0f));
        cubeParent->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(3.0f));

        backgroundCube->addComponent<CubeMesh>();
        backgroundCube->addComponent<Transform>();
        backgroundCube->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(0.0f, 0.0f, -3.0f);
        backgroundCube->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(3.0f, 3.0f, 3.0f);
        backgroundCube->addComponent<BoxCollider>();
        backgroundCube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(glm::vec3(0.0f, 0.0f, -3.0f));
        backgroundCube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(3.0f, 3.0f, 3.0f));

        rightCube->addComponent<CubeMesh>();
        rightCube->addComponent<Transform>();
        rightCube->getComponent<Transform>(ComponentType::TRANSFORM)->z_rotation_angle = 0.0f;
        rightCube->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(0.0f, 0.0f, 0.0f);
        rightCube->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(0.33f);
        rightCube->addComponent<BoxCollider>();
        rightCube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(glm::vec3(3.0f, 0.0f, 0.0f));
        rightCube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(1.0f));

        rightCubeParent->addComponent<CubeMesh>();
        rightCubeParent->addComponent<Transform>();
        rightCubeParent->getComponent<Transform>(ComponentType::TRANSFORM)->z_rotation_angle = 0.0f;
        rightCubeParent->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(3.0f, 0.0f, 0.0f);
        rightCubeParent->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(3.0f);
        rightCubeParent->addComponent<BoxCollider>();
        rightCubeParent->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(glm::vec3(3.0f, 0.0f, 0.0f));
        rightCubeParent->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(3.0f));

        sphere->addComponent<SphereMesh>();
        sphere->addComponent<Transform>();
        sphere->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(3.0f);
        sphere->addComponent<SphereCollider>();
        sphere->addComponent<LightSource>();
        sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(glm::vec3(1.0f));
        sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setRadius(1.0f);

        spotLight->addComponent<LightSource>();
        LightSource* slight = spotLight->getComponent<LightSource>(ComponentType::LIGHTSOURCE);
        slight->setLightType(LightType::SPOT);
        slight->setPosition(glm::vec3(0.0f, 0.0f, 4.0f));
        slight->setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
        slight->setCutOff(12.5f);
        slight->setOuterCutOff(15.0f);
        slight->setConstant(1.0f);
        slight->setLinear(0.35f);
        slight->setQuadratic(0.44f);
        slight->setAmbient(glm::vec3(0.8f, 0.5f, 0.8f)); //TODO: colors.h
        slight->setDiffuse(glm::vec3(0.2f, 0.6f, 0.8f));
        slight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));

        dirLight->addComponent<LightSource>();
        LightSource* dlight = dirLight->getComponent<LightSource>(ComponentType::LIGHTSOURCE);
        dlight->setLightType(LightType::DIRECTIONAL);
        dlight->setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
        dlight->setAmbient(glm::vec3(0.1f, 0.05f, 0.05f));
        dlight->setDiffuse(glm::vec3(0.4f, 0.8f, 0.5f));
        dlight->setSpecular(glm::vec3(0.3f, 0.2f, 0.1f));

        //TODO: funckja w klasie SceneGraphNode (wymog transforma)
        //do zakomentowania jesli chcemy uzyc tej drugiej kamery
        //sphere->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position += 100.0f;
    }  

    basicShader.use();

    std::vector<float> vertices = cube->getComponent<CubeMesh>(ComponentType::CUBEMESH)->getVertices();
    std::vector<float> vertices2 = sphere->getComponent<SphereMesh>(ComponentType::SPHEREMESH)->getVertices();
    std::vector<float> indices = sphere->getComponent<SphereMesh>(ComponentType::SPHEREMESH)->getIndices();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    camera.SetProjMatrix(SCR_WIDTH, SCR_HEIGHT, -100.0f, 100.0f);

    bool debugCamera = true, falling = false; //zmienne do imgui ale nie ma kursora i tak
    float cameraY = 0.0f;
    float scale = 1.0f;

    GLint layers = 100;
    GLint circumferenceTiles = 100;
    std::vector<GLfloat> va;
    std::vector<GLfloat> ia;
    const double PI = 3.141592653589793238463;
    va.reserve((layers + 1) * (circumferenceTiles + 1) * 3);  // 3 floats: x, y, z
    const float pi = 3.1414927f;

    va.reserve((layers + 1)* (circumferenceTiles + 1) * 3);  // 3 floats: x, y, z
    for (int i = 0; i <= layers; ++i)
    {
        float layer_rel = (float)i / (float)layers;
        float layer_ang = (1.0f - 2.0f * layer_rel) * pi / 2.0f;
        float layer_sin = std::sin(layer_ang);
        float layer_cos = std::cos(layer_ang);
        for (int ic = 0; ic <= circumferenceTiles; ic++)
        {
            float circum_rel = (float)ic / (float)circumferenceTiles;
            float cricum_ang = circum_rel * 2.0f * pi - pi;
            float circum_sin = std::sin(cricum_ang);
            float circum_cos = std::cos(cricum_ang);

            va.push_back(layer_cos * circum_cos); // x
            va.push_back(layer_cos * circum_sin); // y
            va.push_back(layer_sin);              // z
        }
    }

    // create the face indices
    ia.reserve(layers * circumferenceTiles * 6);
    for (int il = 0; il < layers; ++il)
    {
        for (int ic = 0; ic < circumferenceTiles; ic++)
        {
            int i0 = il * (circumferenceTiles + 1) + ic;
            int i1 = i0 + 1;
            int i3 = i0 + circumferenceTiles + 1;
            int i2 = i3 + 1;

            int faces[]{ i0, i1, i2, i0, i2, i3 };
            ia.insert(ia.end(), faces + (il == 0 ? 3 : 0), faces + (il == layers - 1 ? 3 : 6));
        }
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, va.size() * sizeof(GLfloat), va.data(),
        GL_STATIC_DRAW);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ia.size() * sizeof(GLuint), ia.data(), GL_STATIC_DRAW);

    GLuint v_attr_inx = 0;
    glVertexAttribPointer(v_attr_inx, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_attr_inx);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(xPos, yPos, zPos);
    sphere1->getGameObject()->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position);
    sphere1->updateTransform();
    sphere1->update(nullptr, false);
    //cube1->updateTransform();
    //cube1->update(nullptr, false);
    root->updateTransform();
    root->update(nullptr, false);

    //shadowMap

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

        //DEBUG
        if (move) {
            root->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(testScale, testScale, testScale);
            //sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(xPos, yPos, zPos);
            //sphere1->getGameObject()->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position);
            root->updateTransform();
            root->update(nullptr, false);
            //cube2->updateTransform();
            //cube2->update(cube1->getGameObject(), false);
            sphere1->updateTransform();
            sphere1->update(nullptr, false);
            move = false;
        }

        //COLLISION
        {
            
            BoxCollider* cubeCollider = cubeParent->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER);
            cubeCollider->z_rotation_angle = 0.0f;
            SphereCollider* sphereCollider = sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER);

            glm::vec4 separate = sphereCollider->isCollision(cubeCollider, false);

            if (separate.w == 1) {

                printf("COLLIDING\n");

                cubeCollider = cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER);
                cubeCollider->z_rotation_angle = 45.0f;
                separate = sphereCollider->isCollision(cubeCollider, true);

                xPos += separate.x;
                yPos += separate.y;
                zPos += separate.z;

                //cubeCollider = leftCube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER);
                //cubeCollider->z_rotation_angle = 0.0f;
                //separate = sphereCollider->isCollision(cubeCollider, true);

                //xPos += separate.x;
                //yPos += separate.y;
                //zPos += separate.z;
            }

            cubeCollider = rightCube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER);

            separate = sphereCollider->isCollision(cubeCollider, true);

            xPos += separate.x;
            yPos += separate.y;
            zPos += separate.z;

            move = true;
               
        }

        sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(xPos, yPos, zPos);
        sphere1->getGameObject()->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position);
        
        printf("%f %f %f\n", sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position.x, sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position.y, sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position.z);

        int display_w, display_h;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);

        //TODO: controls in ImGui
        useDebugCamera(proj, view, window, scale);
        //useOrthoCamera(proj, view, window, cameraY, scale);

        //TODO: renderManager
        basicShader.use();

        {
            LightSource* slight = spotLight->getComponent<LightSource>(ComponentType::LIGHTSOURCE);
            LightSource* dlight = dirLight->getComponent<LightSource>(ComponentType::LIGHTSOURCE);

            basicShader.setVec3("viewPos", camera.Position);

            slight->setLight(basicShader);
            dlight->setLight(basicShader);
        }

        basicShader.setFloat("scale", scale);
        basicShader.setMat4("projection", proj);
        basicShader.setMat4("view", view);

        //cube1parent->render(basicShader);
        //cube4->render(basicShader);
        cube1->render(basicShader);
        cube2->render(basicShader);
        cube3->render(basicShader);

        glm::mat4 model = sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->worldMatrix;
        basicShader.setMat4("model", model);

        glBindVertexArray(sphere1->getGameObject()->getComponent<SphereMesh>(ComponentType::SPHEREMESH)->getVAO()); //TODO: blad jesli brak komponentu
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices2.size(), &vertices2.front(), GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, vertices2.size());

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

void useOrthoCamera(glm::mat4 &proj, glm::mat4 &view, GLFWwindow * window, float &cameraY, float &scale) {

    scale = 100.0f;
    cameraY -= 0.2f;

    proj = camera.GetProjMatrix();
    view = camera.GetOrthoViewMatrix();

    //strzalki
    glfwSetKeyCallback(window, key_callback);

    //esc zamyka okno
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //obrot kamery
//    camera.ProcessMovement(6.0f, 1.5f, rotate, cameraY); //ze spadaniem
    camera.ProcessMovement(6.0f, 1.5f, rotate, 0.0f); //bez spadania
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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        testScale += 0.1;
        move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        testScale-=0.1;
    move = true;
}
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        testScale2+=0.1;
    move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        testScale2-=0.1;
    move = true;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
        yPos += 0.1;
    move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        yPos -= 0.1;
    move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        xPos += 0.1;
    move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        xPos -= 0.1;
    move = true;
    }

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
        rotate = 1;
    }

    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        rotate = -1;
    }
}