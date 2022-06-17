#include "imgui.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <cstdio>
//#include <ft2build.h>
//#include FT_FREETYPE_H

#include "GameObject.h"
#include "CubeMesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Shader.h"
#include "SphereMesh.h"
#include "SceneGraphNode.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Player.h"
#include "LightSource.h"
#include "Model.h"
#include "Bone.h"
#include "Animation.h"
#include "Animator.h"
#include "Frustum.h"
#include "typedefs.h"
#include "GameState.h"
#include "hudRect.h"
#include "Texture.h"
#include "LampAI.h"
#include "Compass.h"
#include "TextRenderer.h"
#include "LevelGenerator.h"
#include "PickaxeAI.h"
#include "LarvaAI.h"
#include "SpawnerAI.h"

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


const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int display_w, display_h;


bool debugBox = 0;
bool skybox = 1;

Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY,
                                float zNear, float zFar)
{
    Frustum     frustum;
    const float halfVSide = zFar * tanf(fovY * .5f);
    const float halfHSide = halfVSide * aspect;
    const glm::vec3 frontMultFar = zFar * cam.Front;
    //printf("Cam position Y: %f\n", cam.Position.y);

    frustum.nearFace = { glm::vec3(0.0f, 0.0f, 1.0f),
                         glm::vec3(0.0f, 0.0f, -1.0f) };
    frustum.farFace = { glm::vec3(0.0f, 0.0f, -1.0f),
                         glm::vec3(0.0f, 0.0f, 1.0f) };
    frustum.rightFace = { glm::vec3(1.0f, 0.0f, 0.0f),
                         glm::vec3(-1.0f, 0.0f, 0.0f) };
    frustum.leftFace = { glm::vec3(-1.0f, 0.0f, 0.0f),
                         glm::vec3(1.0f, 0.0f, 0.0f)};
    frustum.topFace = { glm::vec3(0.0f, 1, 0.0f),
                            glm::vec3(0, -1, 0)};
    frustum.bottomFace = { glm::vec3(0.0f, -1, 0.0f),
                            glm::vec3(0, 1, 0)};

    return frustum;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    //unsigned char* data = stbi_load(faces[0].c_str(), &width, &height, &nrChannels, 0);
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //stbi_image_free(data);

    return textureID;
}

std::vector<std::string> faces
{
        //"./res/textures/right.jpg",
        //"./res/textures/left.jpg",
        //"./res/textures/top.jpg",
        //"./res/textures/bottom.jpg",
        //"./res/textures/front.jpg",
        //"./res/textures/back.jpg"
        "./res/textures/groundbox.jpg",
        "./res/textures/groundbox.jpg",
        "./res/textures/groundbox.jpg",
        "./res/textures/groundbox.jpg",
        "./res/textures/groundbox.jpg",
        "./res/textures/groundbox.jpg"
};

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
void useDebugCamera(glm::mat4 &proj, glm::mat4 &view, GLFWwindow * window, float &scale);
void useOrthoCamera(glm::mat4 &proj, glm::mat4 &view, GLFWwindow * window, float &cameraY, float &scale, Player player);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

GameState state(MAIN_MENU);

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool restart = true;

//camera var
int rotate = 0;
float clipWidth = 80.0f;
float depthPos = 0.0f;
float rotationSpeed = 70.0f;

float r = 0.4f;

//DEBUG
float testScale = 1;
float testScale2 = 1;
float xPos = 0.0;
float yPos = 0.0;
float zPos = 0.0;
bool collision = false;
bool move = 0;
glm::mat4 projection = glm::ortho(0.0f, 720.0f, 0.0f, 1280.0f);
float cubeVertices[] = {
    // positions          // normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Deeper", NULL, NULL);
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
    FMOD::System* ssystem = NULL;

    result = FMOD::System_Create(&ssystem);      // Create the main system object.
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return(-1);
    }

    result = ssystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return(-1);
    }

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Shader basicShader("./res/shaders/basic.vert", "./res/shaders/basic.frag");
    Shader skyboxShader("./res/shaders/cubemap.vert", "./res/shaders/cubemap.frag");
    Shader reflShader("./res/shaders/reflect.vert", "./res/shaders/reflect.frag");

    // LIGHTS
    std::vector<glm::vec3> lightPositions = {
            glm::vec3(-20.0f * 30.0f, 10.0f, -30.0f),
            glm::vec3(-15.0f * 30.0f, 10.0f, -30.0f),
            glm::vec3(-10.0f * 30.0f, 10.0f, -30.0f),
            glm::vec3(-5.0f * 30.0f, 10.0f, -30.0f),
            glm::vec3(0.0f * 30.0f, 10.0f, -30.0f),
            glm::vec3(5.0f * 30.0f, 10.0f, -30.0f),
            glm::vec3(10.0f * 30.0f, 10.0f, -30.0f),
            glm::vec3(15.0f * 30.0f, 10.0f, -30.0f),
            glm::vec3(20.0f * 30.0f, 10.0f, -30.0f),
    };

    glm::vec3 lightColors[] = {
            glm::vec3(180.0f, 180.0f, 180.0f),
            glm::vec3(180.0f, 180.0f, 180.0f),
            glm::vec3(180.0f, 180.0f, 180.0f),
            glm::vec3(180.0f, 180.0f, 180.0f),
            glm::vec3(180.0f, 180.0f, 180.0f),
            glm::vec3(180.0f, 180.0f, 180.0f),
            glm::vec3(180.0f, 180.0f, 180.0f),
            glm::vec3(180.0f, 180.0f, 180.0f),
            glm::vec3(180.0f, 180.0f, 180.0f),
    };

    float lightStrength = 100.0f;

    //-------- GAME OBJECTS --------//


    nodePtr root(new SceneGraphNode());
    nodePtr cube1(new SceneGraphNode());
    nodePtr sphere1(new SceneGraphNode());
    nodePtr caveNode(new SceneGraphNode());

    gameObjectPtr cube(new GameObject());
    gameObjectPtr sphere(new GameObject());
//    gameObjectPtr cave(new GameObject());
    gameObjectPtr lamp(new GameObject());
    gameObjectPtr pickaxe(new GameObject());
    gameObjectPtr spawners(new GameObject());

//    componentPtr caveModel(new Model("./res/models/Colliders/cave1floor.fbx", true));
    componentPtr lampModel(new Model("./res/models/Colliders/cave1_lampColliders.fbx", true));
    componentPtr pickaxeModel(new Model("./res/models/Kilof/kilof2.fbx"));

    componentPtr spawnerColliders(new Model("./res/models/Colliders/spawnerColliders.fbx", true));
    componentPtr larvaModel(new Model("./res/models/Box/box.fbx"));

    //TODO: zmienic przypisanie kilofa do gracza
    pickaxe->addComponent(pickaxeModel, pickaxe);
    pickaxe->addComponent<PickaxeAI>(pickaxe);
    pickaxe->addComponent<SphereCollider>(pickaxe);
    pickaxe->tag = Tag::PICKAXE;
    Player player("./res/models/Box/box.fbx", pickaxe);
    player.getBody()->transform.scale = glm::vec3(2.0f);
//    player.getBody()->transform.position = glm::vec3(577.066101f, 309.0f, -586.7f);
    player.getBody()->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    player.getBody()->transform.y_rotation_angle = 90.0f;

    //-------- GAMEOBJECT VECTORS --------//

    std::vector<gameObjectPtr> larvas;

    //-------- OBJECT PARAMETERS --------//
    {
        cube1->addGameObject(cube);
        sphere1->addGameObject(sphere);

        cube->addComponent<CubeMesh>(cube);
        cube->addComponent<Transform>(cube);
        cube->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(22.486704, 156.337921, 21.386129);
        cube->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(-98.943275f, 0.0f, -28.722950f);
        cube->getComponent<Transform>(ComponentType::TRANSFORM)->z_rotation_angle = 0.0f;
        cube->getComponent<Transform>(ComponentType::TRANSFORM)->x_rotation_angle = 0.0f;
        cube->addComponent<BoxCollider>(cube);
        cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(glm::vec3(0.0f));
        cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(94.5f, 4.0f, 17.0f));

        sphere->addComponent<SphereMesh>(sphere);
        sphere->addComponent<Transform>(sphere);
        sphere->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(3.0f);
        sphere->addComponent<SphereCollider>(sphere);
        sphere->addComponent<LightSource>(sphere);
        sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(glm::vec3(1.0f));
        sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setRadius(1.0f);

        lamp->addComponent(lampModel, lamp);
        lamp->getComponent<Model>(ComponentType::MODEL)->transform.scale = glm::vec3(50.0f);
        lamp->getComponent<Model>(ComponentType::MODEL)->transform.position = glm::vec3(0.0f, -1450.0f, 0.0f);
        lamp->addComponent<LampAI>(lamp);
        lamp->getComponent<LampAI>(ComponentType::AI)->lights = &lightPositions;
        lamp->getComponent<LampAI>(ComponentType::AI)->lit = new std::vector<bool>(lightPositions.size(), 0);

//        cave->addComponent(caveModel, cave);
//        cave->getComponent<Model>(ComponentType::MODEL)->transform.scale = glm::vec3(50.0f);
//        cave->getComponent<Model>(ComponentType::MODEL)->transform.position = glm::vec3(0.0f, -1450.0f, 0.0f);

        pickaxe->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setRadius(50.0f);
        pickaxe->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(pickaxe->getComponent<Model>(ComponentType::MODEL)->transform.position);

        //LarvaAI::instantiateLarva(&larvas, &lightPositions, larvaModel, glm::vec3(581.819336f, 303.709015f, -582.5f));

        spawners->addComponent(spawnerColliders, spawners);
        spawners->getComponent<Model>(ComponentType::MODEL)->transform.scale = glm::vec3(50.0f);
        spawners->getComponent<Model>(ComponentType::MODEL)->transform.position = glm::vec3(0.0f, -1450.0f, 0.0f);
        spawners->addComponent<SpawnerAI>(spawners);
        spawners->getComponent<SpawnerAI>(ComponentType::AI)->larvaModel = larvaModel;
        spawners->getComponent<SpawnerAI>(ComponentType::AI)->larvas = &larvas;
        spawners->getComponent<SpawnerAI>(ComponentType::AI)->lights = &lightPositions;
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
    camera.SetProjMatrix(SCR_WIDTH, SCR_HEIGHT, -SCR_WIDTH, SCR_WIDTH);

    float cameraY = 0.0f;
    float scale = 1.0f;

    Shader PBRShader("./res/shaders/PBR.vert", "./res/shaders/PBR.frag");
    PBRShader.use();
    PBRShader.setInt("albedoMap", 0);
    PBRShader.setInt("normalMap", 1);
    PBRShader.setInt("metallicMap", 2);
    PBRShader.setInt("roughnessMap", 3);
    PBRShader.setInt("aoMap", 4);
    PBRShader.setInt("emissiveMap", 5);

    Shader skeletonShader("./res/shaders/PBRskeleton.vert", "./res/shaders/PBRskeleton.frag");
    skeletonShader.use();
    skeletonShader.setInt("albedoMap", 0);
    skeletonShader.setInt("normalMap", 1);
    skeletonShader.setInt("metallicMap", 2);
    skeletonShader.setInt("roughnessMap", 3);
    skeletonShader.setInt("aoMap", 4);
    skeletonShader.setInt("emissiveMap", 5);

    //-------- LEVEL RESOURCES --------//

    Model level("./res/models/cave1/cave1_nr_mdl.fbx");
    level.transform.scale = glm::vec3(50.0f);
    level.transform.position = glm::vec3(0.0f, -1460.0f, 0.0f);

    Model LRcolliders("./res/models/Colliders/LRWalls.fbx", true);
    LRcolliders.transform.scale = glm::vec3(50.0f);
    LRcolliders.transform.position = glm::vec3(0.0f, -1450.0f, 0.0f);

    Model FBcolliders("./res/models/Colliders/FBWalls.fbx", true);
    FBcolliders.transform.scale = glm::vec3(50.0f);
    FBcolliders.transform.position = glm::vec3(0.0f, -1450.0f, 0.0f);

    Model floor1("./res/models/Colliders/cave1floor.fbx", true);
    floor1.transform.scale = glm::vec3(50.0f);
    floor1.transform.position = glm::vec3(0.0f, -1450.0f, 0.0f);

    Model lamps("./res/models/lampka/cave1Lamps.fbx");
    lamps.transform.scale = glm::vec3(50.0f);
    lamps.transform.position = glm::vec3(0.0f, -1460.0f, 0.0f);


    //-------- LEVEL GENERATOR SET UP --------//
    std::vector<Model> caveModels = {level, level, level};
    std::vector<std::vector<Model>> walls = {{LRcolliders, FBcolliders}, {LRcolliders, FBcolliders}, {LRcolliders, FBcolliders}};
    std::vector<Model> floors = {floor1, floor1, floor1};
    std::vector<Model> lampModels = {lamps, lamps, lamps};
    std::vector<Model> lampColliders = {*lamp->getComponent<Model>(ComponentType::MODEL), *lamp->getComponent<Model>(ComponentType::MODEL), *lamp->getComponent<Model>(ComponentType::MODEL)};

    LevelGenerator gen(caveModels, walls, floors, lampModels, lampColliders);
//    gen.newGame(SCR_HEIGHT);


    Frustum camFrustum = createFrustumFromCamera(camera,
        (float)SCR_WIDTH / (float)SCR_HEIGHT, 180, 0.1f, 100.0f);

    //    Model zombieAnimTestModel("./res/models/zombie/zombieAnimTest.fbx");
    //    zombieAnimTestModel.transform.scale = glm::vec3(2.5f);
    //    zombieAnimTestModel.transform.position = glm::vec3(0.0f, -200.0f, 0.0f);

    //    Model simpleModel("./res/models/zombie/zombieCorrect.fbx");
    //    simpleModel.transform.scale = glm::vec3(1.5f);
    //    simpleModel.transform.position = glm::vec3(0.0f, -200.0f, 20.0f);
    //    Animation animation("./res/models/zombie/zombieCorrect.fbx", &simpleModel);
    //    Model simpleModel("./res/models/debug/simpleAnimAF.fbx");
    //    simpleModel.transform.scale = glm::vec3(3.5f);
    //    simpleModel.transform.position = glm::vec3(0.0f, 000.0f, 0.0f);
    //    simpleModel.transform.x_rotation_angle = 90.0f;
    //    Animation animation("./res/models/debug/simpleAnimAF.fbx", &simpleModel);
    //    Animator animator(&animation);

    unsigned int cubemapTexture;
    if (skybox) {
        cubemapTexture = loadCubemap(faces);
        reflShader.use();
        reflShader.setInt("skybox", 0);

        skyboxShader.use();
        skyboxShader.setInt("skybox", 0);
    }

    //DEBUG
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //-------- HUD RESOURCES --------//
    int old_w, old_h;
    Shader hudShader("./res/shaders/basicTexture.vert", "./res/shaders/basicTexture.frag");
    hudShader.use();
    hudShader.setMat4("proj", camera.GetHudProjMatrix(SCR_WIDTH, SCR_HEIGHT));

    Compass compass(SCR_WIDTH, SCR_HEIGHT, hudShader);
    TextRenderer points(camera.GetHudProjMatrix(SCR_WIDTH, SCR_HEIGHT));

    state.setState(GAME_RUNNING);

    printf("%f\n", player.getBody()->transform.position.y);
    bool firstFrame = true;


    //-------- MAIN LOOP --------//
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        old_w = display_w;
        old_h = display_h;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glViewport(0, 0, display_w, display_h);

        if (display_w != old_w || display_h != old_h) {
            state.setSetUp(true);
            compass.init(display_w, display_h, PBRShader);
        }

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        switch (state.getCurState()) {
        case GAME_RUNNING: {

            if (restart) {
                gen.newGame(SCR_HEIGHT);
                player.newGame();
                cameraY = 0.0f;
                depthPos = 0.0f;
                camera.newGame();
                restart = false;
                points.newGame();
            }


            state.gameRunning(window);
            //              animator.UpdateAnimation(deltaTime);



            //-------- ImGui --------//
            {
                ImGui::Begin("Debug");
                ImGui::Text("points: %i", points.getPoints());
                ImGui::NewLine();

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

                ImGui::Text("player's position x: %f  | y: %f | z: %f", player.getBody()->transform.position.x,
                                player.getBody()->transform.position.y, player.getBody()->transform.position.z);
                ImGui::Text("pickaxe's position x: %f  | y: %f | z: %f", pickaxe->getComponent<Model>(ComponentType::MODEL)->transform.position.x,
                            pickaxe->getComponent<Model>(ComponentType::MODEL)->transform.position.y, pickaxe->getComponent<Model>(ComponentType::MODEL)->transform.position.z);
                ImGui::Text("depthPos: %f   clipWidth: %f", depthPos, clipWidth);
                ImGui::Text("x velocity: %f | z velocity: %f", player.getVelX(), player.getVelZ());
                ImGui::Text("player's angle: %f | camera's angle: %f", player.getAngle(), camera.getAngle());
                ImGui::NewLine();
                ImGui::Text("camera's direction: %i ", (int)camera.getCameraDirection());
                ImGui::Text("camera's position x: %f  | y: %f | z: %f", camera.getCamPos().x,
                                camera.getCamPos().y, camera.getCamPos().z);
                ImGui::NewLine();
                ImGui::SliderFloat("zoom", &r, 0.1f, 1.0f);
                ImGui::NewLine();
                ImGui::Text("which cave %i", gen.whereAmI(player.getBody()->transform.position.y));
                if(ImGui::Button("New game")) {
                    restart = true;
                }

                ImGui::End();
                }



            //useDebugCamera(proj, view, window, scale);
            useOrthoCamera(proj, view, window, cameraY, scale, player);

            //liczenie punktow
            points.checkPoints(lamp->getComponent<LampAI>(ComponentType::AI)->lit, (int)player.getBody()->transform.position.y);

            //game over jesli gracz wyjdzie poza gorna krawedz kamery
//            if(camera.isGameover())
//                state.setState(GAME_OVER);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            pickaxe->getComponent<PickaxeAI>(ComponentType::AI)->update(window, deltaTime);

            lightPositions[0] = player.getBody()->transform.position;

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            if (debugBox)
            {
                basicShader.use();
                basicShader.setFloat("scale", scale);
                basicShader.setMat4("projection", proj);
                basicShader.setMat4("view", view);

                glm::mat4 model = cube1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->getCombinedMatrix();
                basicShader.setMat4("model", model);

                glBindVertexArray(cube1->getGameObject()->getComponent<CubeMesh>(ComponentType::CUBEMESH)->getVAO());
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(skyboxVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            //-------- PHYSICS --------//

//                camera.AdjustPlanes(SCR_WIDTH * r, SCR_HEIGHT * r, depthPos, 5000.0f, 5000.0f);
                if (rotate == 0) {
                    player.move(window, camera.getCameraDirection(), deltaTime, depthPos);
                    camera.AdjustPlanes(SCR_WIDTH * r, SCR_HEIGHT * r, depthPos, 0.0f, clipWidth);
                }
                else {
                    player.stop(); //gracz zatrzymywany przy obrocie kamery
                    camera.AdjustPlanes(SCR_WIDTH * r, SCR_HEIGHT * r, depthPos, 5000.0f, 5000.0f);
                }

            //gracz obrocony jedna strona do kamery
            player.rotate(-camera.getAngle());

            for (int i = 0; i < 1; i++)
            {
                if(!firstFrame) player.gravityOn(deltaTime);
                //-------- COLLISIONS --------//
//                player.checkCollision(cave->getComponent<Model>(ComponentType::MODEL)->getColliders());

                gen.collisions(&player);

                //-------- TRIGGERS --------//
//                player.detectCollision(lamp->getComponent<Model>(ComponentType::MODEL)->getColliders());
//                player.detectCollision(spawners->getComponent<Model>(ComponentType::MODEL)->getColliders());
                gen.triggers(&player);

//                for (auto larva : larvas) {
//
//                    player.detectCollision(larva->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER));
//                    larva->getComponent<LarvaAI>(ComponentType::AI)->update(window, deltaTime);
//                    larva->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->separate(cave->getComponent<Model>(ComponentType::MODEL)->getColliders());
//                    larva->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->checkTrigger(lamp->getComponent<Model>(ComponentType::MODEL)->getColliders());
//                    if (pickaxe->getComponent<PickaxeAI>(ComponentType::AI)->isThrown) {
//                        pickaxe->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->checkTrigger(larva->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER));
//                    }
//
//                }


            }

            firstFrame = false;

            //printf("LARVAS: %d\n", larvas.size());

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            PBRShader.use();
            PBRShader.setMat4("projection", proj);
            PBRShader.setMat4("view", view);
            PBRShader.setVec3("camPos", camera.Position);
            PBRShader.setFloat("scale", scale);
            PBRShader.setFloat("lightStrength", lightStrength);

            for (unsigned int i = 0; i < lightPositions.size(); ++i)
            {
                glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
                newPos = lightPositions[i];
                PBRShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
                PBRShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
            }

            camFrustum = createFrustumFromCamera(camera, (float)SCR_WIDTH / (float)SCR_HEIGHT, 180, -100.0f, 100.0f);



            //-------- DRAW --------//
            {
//                level.Draw(PBRShader);
//                lamps.Draw(PBRShader);
                //LRcolliders.Draw(PBRShader);
                //FBcolliders.Draw(PBRShader);
//                cave->getComponent<Model>(ComponentType::MODEL)->Draw(PBRShader);
                //lamp->getComponent<Model>(ComponentType::MODEL)->Draw(PBRShader);
//                spawners->getComponent<Model>(ComponentType::MODEL)->Draw(PBRShader);

//                floor1.Draw(PBRShader);

                gen.update(camera.getCamPos().y);
                gen.DrawLevels(PBRShader);

                for (auto larva : larvas) {

                    larva->getComponent<Model>(ComponentType::MODEL)->Draw(PBRShader);

                }

                player.render(PBRShader);
            }



            // Animation model
//            skeletonShader.use();
//            skeletonShader.setMat4("projection", proj);
//            skeletonShader.setMat4("view", view);
//            skeletonShader.setVec3("camPos", camera.Position);
//            skeletonShader.setFloat("scale", scale);

//            auto transforms = animator.GetFinalBoneMatrices();
//            for (int i = 0; i < transforms.size(); ++i)
//                skeletonShader.setMat4("bones[" + std::to_string(i) + "]", transforms[i]);

            //simpleModel.Draw(skeletonShader);

            //SKYBOX
            if (skybox)
            {
                useDebugCamera(proj, view, window, scale);
                glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
                skyboxShader.use();
                skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
                skyboxShader.setMat4("projection", proj);
                // skybox cube
                glBindVertexArray(skyboxVAO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                glDepthFunc(GL_LESS); // set depth function back to default
            }

            //kompas (dziala tylko jak przed jest renderowany skybox XD)
            hudShader.use();
            hudShader.setMat4("proj", glm::ortho(-(display_w/2.0f), display_w/2.0f, -(display_h/2.0f), display_h/2.0f, -10.0f, 10.0f));
            compass.Draw(hudShader, camera.getAngle());

            break;
        }
        case MAIN_MENU: {
            hudShader.use();
            hudShader.setMat4("proj", camera.GetHudProjMatrix(display_w, display_h));

            state.mainMenu(window, hudShader);

            restart = true;

            break;
        }
        case PAUSE: {
            if(skybox) {
                glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
                skyboxShader.use();
                skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
                skyboxShader.setMat4("projection", proj);
                // skybox cube
                glBindVertexArray(skyboxVAO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                glDepthFunc(GL_LESS); // set depth function back to default
            }

            hudShader.use();
            hudShader.setMat4("proj", camera.GetHudProjMatrix(display_w, display_h));

            state.pause(window, hudShader);

            break;
        }
        case CREDITS: {
            hudShader.use();
            hudShader.setMat4("proj", camera.GetHudProjMatrix(display_w, display_h));

            state.credits(window, hudShader);

            break;
        }
        case GAME_OVER: {

            {
                ImGui::Begin("SCORE");
                ImGui::Text("points: %i", points.getPoints());
                ImGui::End();
            }

            hudShader.use();
            hudShader.setMat4("proj", camera.GetHudProjMatrix(display_w, display_h));

            state.gameOver(window, hudShader);

            restart = true;

            break;
        }
        case EXIT: {
            glfwSetWindowShouldClose(window, true);
        }
        }

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


///CAMERA TYPE FUNCTIONS
void useDebugCamera(glm::mat4 &proj, glm::mat4 &view, GLFWwindow * window, float &scale) {
    scale = 1.0f;
    //processInput(window);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
}

void useOrthoCamera(glm::mat4 &proj, glm::mat4 &view, GLFWwindow * window, float &cameraY, float &scale, Player player) {

    scale = 100.0f;
//    cameraY -= 0.05f;

    ///DEBUG
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cameraY += 5.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cameraY -= 5.0f;
    }

    proj = camera.GetProjMatrix();
    view = camera.GetOrthoViewMatrix();

    glfwSetKeyCallback(window, key_callback);

    //obrot kamery
    camera.ProcessMovement(6.0f, rotationSpeed * deltaTime, rotate, cameraY, depthPos, player.getBody()->transform.position);
//    camera.AdjustPlanes(SCR_WIDTH, SCR_HEIGHT, depthPos, clipZ);
}

///INPUT HANDLING FUNCTIONS
void processInput(GLFWwindow* window)
{
    //esc = pause lub esc = exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
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

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        yPos = 10.0f;
        xPos = 0.0f;
        printf("%f\n", yPos);
    move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        yPos = -10.0f;
        xPos = 0.0f;
    move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        xPos = 10.0f;
        yPos = 0.0f;
    move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        xPos = -10.0f;
        yPos = 0.0f;
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

    state.processMouse(lastX, (float)display_h - lastY);
    //camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//obracanie kamerą
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(rotate == 0) {
        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
            rotate = 1;
        } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            rotate = -1;
        }
    }

}

//obsluga huda
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        state.processMouseButton(true);
    else
        state.processMouseButton(false);
}