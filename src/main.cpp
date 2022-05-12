#include "imgui.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
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


const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

struct Plan
{
    // unit vector
    glm::vec3 normal = { 0.f, 1.f, 0.f };

    // distance from origin to the nearest point in the plan
    float     distance = 0.f;

    Plan() = default;

    Plan(const glm::vec3 & p1, const glm::vec3 & norm)
            : normal(glm::normalize(norm)),
              distance(glm::dot(normal, p1))
    {}

    float getSignedDistanceToPlan(const glm::vec3 & point) const
    {
        return glm::dot(normal, point) - distance;
    }

};

struct Frustum
{
    Plan topFace;
    Plan bottomFace;

    Plan rightFace;
    Plan leftFace;

    Plan farFace;
    Plan nearFace;
};

Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY,
                                float zNear, float zFar)
{
    Frustum     frustum;
    const float halfVSide = zFar * tanf(fovY * .5f);
    const float halfHSide = halfVSide * aspect;
    const glm::vec3 frontMultFar = zFar * cam.Front;
    printf("Cam position Y: %f\n", cam.Position.y);

    frustum.nearFace = { cam.Position + zNear * cam.Front, cam.Front };
    frustum.farFace = { cam.Position + frontMultFar, -cam.Front };
    frustum.rightFace = { cam.Position,
                          glm::cross(cam.Up,frontMultFar + cam.Right * halfHSide) };
    frustum.leftFace = { cam.Position,
                         glm::cross(frontMultFar - cam.Right * halfHSide, cam.Up) };
    frustum.topFace = { glm::vec3(0.0f, 1, 0.0f),
                        glm::vec3(0, 1, 0)};
    frustum.bottomFace = { glm::vec3(0.0f, 1, 0.0f),
                           glm::vec3(0, -1, 0)};

    return frustum;
}

bool isOnOrForwardPlan(BoxCollider b, Plan p, glm::mat4 proj, glm::mat4 view) {

    BoxCollider tmp;

    float divisor;

    tmp.setCenter(proj* view * glm::vec4(b.getCenter(), 1.0f));
    glm::vec3 tmpMaxPoints = proj * view * glm::vec4(b.getMaxX(), b.getMaxY(), b.getMaxZ(), 1.0f);
    tmp.setSize(glm::vec3((tmpMaxPoints.x - tmp.getCenter().x) * 2, (tmpMaxPoints.y - tmp.getCenter().y) * 2, (tmpMaxPoints.z - tmp.getCenter().z) * 2));

    divisor = tmp.getCenter().y / b.getCenter().y;

    printf("tmp center: %f %f %f\n", tmp.getCenter().x, tmp.getCenter().y, tmp.getCenter().z);
    printf("tmp maxPoints: %f %f %f\n", tmpMaxPoints.x, tmpMaxPoints.y, tmpMaxPoints.z);
    printf("tmp extents: %f %f %f\n", tmp.getSizeX(), tmp.getSizeY(), tmp.getSizeZ() / 2);
    printf("b extents: %f %f %f\n", b.getSizeX() / 2, b.getSizeY() / 2, b.getSizeZ() / 2);
    printf("divisor: %f\n", divisor);


    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    float r = tmp.getSizeX()/2 * glm::abs(p.normal.x) + tmp.getSizeY()/2 * glm::abs(p.normal.y) + tmp.getSizeZ()/2 * glm::abs(p.normal.z);

    printf("r: %f\n", r);

    // Compute distance of box center from plane
    float s = glm::dot(p.normal, tmp.getCenter()) - p.distance;

    printf("Box to Plane: %f\n", p.getSignedDistanceToPlan(tmp.getCenter()));

    // Intersection occurs when distance s falls within [-r,+r] interval
    return -r <= p.getSignedDistanceToPlan(tmp.getCenter());

}

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

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//camera var
int rotate = 0;
float clipZ = 720.0f;
float clipWidth = 100.0f;
float depthPos = 0.0f;

//DEBUG
float testScale = 1;
float testScale2 = 1;
float xPos = 0.0;
float yPos = 1.8;
float zPos = 0.0;
bool collision = false;
bool move = 0;
glm::mat4 projection = glm::ortho(0.0f, 720.0f, 0.0f, 1280.0f);


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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

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
    nodePtr cube1(new SceneGraphNode());
    nodePtr sphere1(new SceneGraphNode());

    gameObjectPtr cube(new GameObject());
    gameObjectPtr sphere(new GameObject());



    //OBJECT PARAMETERS
    {
        cube1->addGameObject(cube);
        sphere1->addGameObject(sphere);

        cube->addComponent<CubeMesh>();
        cube->addComponent<Transform>();
        cube->getComponent<Transform>(ComponentType::TRANSFORM)->scale = glm::vec3(36.0f * 30, 6.0f * 30, 6.0f * 30);
        cube->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(0.0f, 1.4f * 30, 0.0f * 30);
        cube->addComponent<BoxCollider>();
        cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setCenter(glm::vec3(0.0f));
        cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER)->setSize(glm::vec3(1.0f));

        sphere->addComponent<SphereMesh>();
        sphere->addComponent<Transform>();
        sphere->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(3.0f);
        sphere->addComponent<SphereCollider>();
        sphere->addComponent<LightSource>();
        sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(glm::vec3(1.0f));
        sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setRadius(1.0f);

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

    sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(xPos, yPos, zPos);
    sphere1->getGameObject()->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position);
    sphere1->updateTransform();
    sphere1->update(nullptr, false);
    cube1->updateTransform();
    cube1->update(nullptr, false);

    // LIGHTS
    glm::vec3 lightPositions[] = {
            glm::vec3(-20.0f, 5.0f, 10.0f),
            glm::vec3(-15.0f, 5.0f, 10.0f),
            glm::vec3(-10.0f, 5.0f, 10.0f),
            glm::vec3(-5.0f, 5.0f, 10.0f),
            glm::vec3(0.0f, 5.0f, 10.0f),
            glm::vec3(5.0f, 5.0f, 10.0f),
            glm::vec3(10.0f, 5.0f, 10.0f),
            glm::vec3(15.0f, 5.0f, 10.0f),
            glm::vec3(20.0f, 5.0f, 10.0f),
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

    Shader PBRShader("./res/shaders/PBR.vert", "./res/shaders/PBR.frag");
    PBRShader.use();
    PBRShader.setInt("albedoMap", 0);
    PBRShader.setInt("normalMap", 1);
    PBRShader.setInt("metallicMap", 2);
    PBRShader.setInt("roughnessMap", 3);
    PBRShader.setInt("aoMap", 4);
    PBRShader.setInt("emissiveMap", 5);

    Model rocks("./res/models/Rocks/rocks.fbx");
    rocks.transform.scale = glm::vec3(30.0f);

    //level
//    Model level("./res/models/caveSystem/caveSystem.fbx");
//
//    level.transform.scale = glm::vec3(30.0f);
//    level.transform.position = glm::vec3(0.0f, -360.0f, 0.0f);

    //Player
    Player player("./res/models/Box/box.fbx");
    player.getBody()->transform.scale = glm::vec3(1.5f);

    Frustum camFrustum = createFrustumFromCamera(camera, (float)SCR_WIDTH / (float)SCR_HEIGHT, 180, 0.1f, 100.0f);

    if (isOnOrForwardPlan(rocks.boundingVolume, camFrustum.topFace, proj, view)) {
        printf("ON\n");
    }

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui
        {
            ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);

            ImGui::Text("Pozycja gracza x: %f  | y: %f | z: %f", player.getBody()->transform.position.x,
                    player.getBody()->transform.position.y, player.getBody()->transform.position.z);

            ImGui::Text("depthPos: %f   clipWidth: %f", depthPos, clipWidth);

            ImGui::End();
        }

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //DEBUG
        if (move) {
            sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position = glm::vec3(xPos, yPos, zPos);
            sphere1->getGameObject()->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(sphere1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->position);
            cube1->updateTransform();
            cube1->update(nullptr, false);
            sphere1->updateTransform();
            sphere1->update(nullptr, false);
            move = false;
        }

        //TODO: w klasie colliderow
        {

            BoxCollider* cubeCollider = cube->getComponent<BoxCollider>(ComponentType::BOXCOLLIDER);
            cubeCollider->z_rotation_angle = 45.0f;
            SphereCollider* sphereCollider = sphere->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER);

            glm::vec3 separate = sphereCollider->isCollision(cubeCollider, true);


            xPos += separate.x;
            yPos += separate.y;
            zPos += separate.z;

            move = true;


        }
        
        int display_w, display_h;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);

        //useDebugCamera(proj, view, window, scale);
        useOrthoCamera(proj, view, window, cameraY, scale);
        //TODO: renderManager/meshComponent

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        basicShader.use();
//
//        {   //TODO: w LightComponent
//            LightSource* slight = sphere->getComponent<LightSource>(ComponentType::LIGHTSOURCE);
//
//            basicShader.setVec3("viewPos", camera.Position);
//
//            basicShader.setVec3("spotLight.position", slight->getPosition());
//            basicShader.setVec3("spotLight.direction", slight->getDirection());
//            basicShader.setVec3("spotLight.ambient", slight->getAmbient());
//            basicShader.setVec3("spotLight.diffuse", slight->getDiffuse());
//            basicShader.setVec3("spotLight.specular", slight->getSpecular());
//            basicShader.setFloat("spotLight.constant", slight->getConstant());
//            basicShader.setFloat("spotLight.linear", slight->getLinear());
//            basicShader.setFloat("spotLight.quadratic", slight->getQuadratic());
//            basicShader.setFloat("spotLight.cutOff", slight->getCutOff());
//            basicShader.setFloat("spotLight.outerCutOff", slight->getOuterCutOff());
//        }
//
        basicShader.setFloat("scale", scale);
        basicShader.setMat4("projection", proj);
        basicShader.setMat4("view", view);

        //cube1->render(basicShader);
        glm::mat4 model = cube1->getGameObject()->getComponent<Transform>(ComponentType::TRANSFORM)->getCombinedMatrix();
        basicShader.setMat4("model", model);

        glBindVertexArray(cube1->getGameObject()->getComponent<CubeMesh>(ComponentType::CUBEMESH)->getVAO());
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertices.size(), &vertices.front(), GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        PBRShader.use();
        PBRShader.setMat4("projection", proj);
        PBRShader.setMat4("view", view);
        PBRShader.setVec3("camPos", camera.Position);
        PBRShader.setFloat("scale", scale);

        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
        {
            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions[i];
            PBRShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            PBRShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
        }

        camFrustum = createFrustumFromCamera(camera, (float)SCR_WIDTH / (float)SCR_HEIGHT, 180, 0.1f, 100.0f);
        if (!isOnOrForwardPlan(rocks.boundingVolume, camFrustum.topFace, proj, view)) {
            rocks.Draw(PBRShader);
        }

        //PLAYER + LEVEL
//        if(!rotate) {
//            player.move(window, camera.getCameraDirection(), deltaTime, depthPos);
//            camera.AdjustPlanes(SCR_WIDTH, SCR_HEIGHT, depthPos, clipWidth);
//        } else {
//            camera.AdjustPlanes(SCR_WIDTH, SCR_HEIGHT, depthPos, SCR_WIDTH);
//        }
//
//        player.render(PBRShader);
//        level.Draw(PBRShader);
//
//        camera.ProcessMovement(6.0f, 50.0f * deltaTime, rotate, 0.0f, depthPos, player.getBody()->transform.position, player.getDirection());
//        camera.AdjustPlanes(SCR_WIDTH, SCR_HEIGHT, depthPos, clipWidth);

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
    camera.ProcessMovementNoPlayer(6.0f, 50.0f * deltaTime, rotate, 0.0f); //bez spadania
    camera.AdjustPlanes(SCR_WIDTH, SCR_HEIGHT, depthPos, clipZ);
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

    //camera.ProcessMouseMovement(xoffset, yoffset);
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
    if(rotate == 0) {
        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
            rotate = 1;
        } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            rotate = -1;
        }
    }
}