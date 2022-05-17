#pragma once
//TODO: wlasna implementacja

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

enum Camera_Facing_Direction {
    FRONT_DIR,
    RIGHT_DIR,
    BACK_DIR,
    LEFT_DIR

};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera_Facing_Direction dir;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        proj = glm::mat4(1.0f);
        view = glm::mat4(1.0f);
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        proj = glm::mat4(1.0f);
        view = glm::mat4(1.0f);
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetOrthoViewMatrix() {
        return view;
    }

    glm::mat4 GetProjMatrix() {
        return proj;
    }

    //do dopasowania po wgraniu levelu
    void SetProjMatrix(float width, float height, float nearPlane, float farPlane) {
        proj = glm::ortho(-(width/2.0f), width/2.0f, -(height/2.0f), height/2.0f, nearPlane, farPlane);
//        proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
    }

    void AdjustPlanes(float width, float height, float zPos, float planeWidth) {
        proj = glm::ortho(-(width/2.0f), width/2.0f, -(height/2.0f), height/2.0f, zPos - planeWidth, zPos + planeWidth);
    }

    glm::mat4 GetMultipliedMatrices() {
        return proj * view;
    }

    void ProcessMovement(float radius, float speed, int &move, float camY, float &zPos, glm::vec3 pos, int direction) {

        if(move != 0) {
            totalDegrees += speed;
            angle += speed * move;

            if(totalDegrees >= 90.0f) {
                //ustalanie kierunku
                int dirInt = (int)dir + move;
                dir = (Camera_Facing_Direction)dirInt;
                if(dir > LEFT_DIR)
                    dir = FRONT_DIR;
                if(dir < FRONT_DIR)
                    dir = LEFT_DIR;

                //TODO: ładniej zrobić
//                if(dir == FRONT_DIR)
//                    zPos = -pos.z;
//                else if(dir == RIGHT_DIR)
//                    zPos = -pos.x;
//                else if(dir == BACK_DIR)
//                    zPos = pos.z;
//                else if(dir == LEFT_DIR)
//                    zPos = -pos.x;

                if(dir == FRONT_DIR || dir == BACK_DIR) {
                    zPos = pos.z * direction;
                }
                else {
                    zPos = pos.x * direction;
                }


                totalDegrees = 0.0f;
                move = 0;

            }

            if(angle >= 360.0f)
                angle = 0.0f;
            else if(angle <= -90.0f)
                angle = 270.0f;
        }

        float camX = sin(glm::radians(angle)) * radius;
        float camZ = cos(glm::radians(angle)) * radius;

        view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, camY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        //clipping plane
//        if(angle == 0.0f || angle == 180.0f)
//            view = glm::translate(view, glm::vec3(0.0f, 0.0f, zPos));
//        else if(angle == 90.0f || angle == 270.0f)
//            view = glm::translate(view, glm::vec3(zPos, 0.0f, 0.0f));

    }

    void ProcessMovementNoPlayer(float radius, float speed, int &move, float camY) {

        if(move != 0) {
            totalDegrees += speed;
            angle += speed * move;

            if(totalDegrees >= 90.0f) {
                //ustalanie kierunku
                int dirInt = (int)dir + move;
                dir = (Camera_Facing_Direction)dirInt;
                if(dir > LEFT_DIR)
                    dir = FRONT_DIR;
                if(dir < FRONT_DIR)
                    dir = LEFT_DIR;

                totalDegrees = 0.0f;
                move = 0;

            }

            if(angle >= 360.0f)
                angle = 0.0f;
            else if(angle <= -90.0f)
                angle = 270.0f;
        }

        float camX = sin(glm::radians(angle)) * radius;
        float camZ = cos(glm::radians(angle)) * radius;
        Position += camY;
        view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, camY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    }

    Camera_Facing_Direction getCameraDirection() {
        return dir;
    }


    /*--------------------DEBUG--------------------*/

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:

    glm::mat4 proj;
    glm::mat4 view;
    float angle = 0.0f;
    float totalDegrees = 0.0f; //pomocnicza

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};