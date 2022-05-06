#include "SphereCollider.h"

#include "BoxCOllider.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

SphereCollider::SphereCollider(glm::vec3 center, float radius) : center(center), radius(radius) {};

SphereCollider::~SphereCollider() {};

glm::vec3 SphereCollider::getCenter() {

	return center;

}

float SphereCollider::getRadius() {

	return radius;

}

void SphereCollider::setCenter(glm::vec3 c) {

	center = c;

}

void SphereCollider::setRadius(float r) {

	radius = r;

}

bool SphereCollider::isCollision(BoxCollider* other) {

    glm::mat4 tmpMat = (
        glm::translate(glm::mat4(1.0f), glm::vec3(other->getCenter())) *
        (glm::rotate(glm::mat4(1.0f), glm::radians(-other->x_rotation_angle), glm::vec3(-1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(-other->y_rotation_angle), glm::vec3(0, -1, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(-other->z_rotation_angle), glm::vec3(0, 0, -1))) *
        glm::translate(glm::mat4(1.0f), glm::vec3(-other->getCenter()))
        );

    glm::vec4 tmpCenter = glm::vec4(center.x, center.y, center.z, 1.0f);

    tmpCenter = tmpMat * tmpCenter;

    float x = glm::max(other->getMinX(), glm::min(tmpCenter.x, other->getMaxX()));
    float y = glm::max(other->getMinY(), glm::min(tmpCenter.y, other->getMaxY()));
    float z = glm::max(other->getMinZ(), glm::min(tmpCenter.z, other->getMaxZ()));

    // this is the same as isPointInsideSphere
    float distance = glm::sqrt((x - tmpCenter.x) * (x - tmpCenter.x) +
        (y - tmpCenter.y) * (y - tmpCenter.y) +
        (z - tmpCenter.z) * (z - tmpCenter.z));

    return distance < getRadius();

}

bool SphereCollider::isType(ComponentType t) {

	return t == ComponentType::SPHERECOLLIDER;

}

void SphereCollider::test() {

	printf("TEST_SPHERECOLLIDER\n");

}