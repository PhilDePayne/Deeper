#include "Transform.h"
#include "LogMacros.h"


#ifdef DEEPER_TRANSFORM_CLASS_LOGS

#define TEST_LOG

#endif


Transform::Transform() :worldMatrix(1.0f){
}

Transform::~Transform() {};

glm::mat4 Transform::getCombinedMatrix() {

    return   glm::translate(glm::mat4(1.0f), position) *
            (glm::rotate(glm::mat4(1.0f), glm::radians(x_rotation_angle), glm::vec3(-1, 0, 0)) *
             glm::rotate(glm::mat4(1.0f), glm::radians(y_rotation_angle), glm::vec3(0, -1, 0)) *
             glm::rotate(glm::mat4(1.0f), glm::radians(z_rotation_angle), glm::vec3(0, 0, -1))
            ) *
             glm::scale(glm::mat4(1.0f), scale);

}

bool Transform::isType(ComponentType t) {

    return (t == ComponentType::TRANSFORM);

}

void Transform::test() {
#ifdef TEST_LOG
    printf("TEST_TRANSFORM\n");
#endif
}
