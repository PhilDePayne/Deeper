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

glm::vec3 SphereCollider::isCollision(BoxCollider* other, bool resolve) {

    glm::vec3 ret(0, 0, 0);

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

    //printf("%f %f %f %f %f %f %f\n", other->getCenter().x, other->getCenter().y, other->getCenter().z,
        //other->getSizeX(), other->getSizeY(), other->getSizeZ(), other->z_rotation_angle);

    if (distance < getRadius() && resolve) {

        printf("COLLISION\n");
        printf("%f %f %f %f %f %f %f\n", other->getCenter().x, other->getCenter().y, other->getCenter().z,
        other->getSizeX(), other->getSizeY(), other->getSizeZ(), other->z_rotation_angle);

        float xDist = (x - tmpCenter.x);
        float yDist = (y - tmpCenter.y);
        float zDist = (z - tmpCenter.z);

        float dist;

        if ((glm::abs(xDist) > glm::abs(zDist)) && (glm::abs(xDist) > glm::abs(yDist))) {

            dist = (getRadius() + other->getSizeX() / 2) - glm::abs(other->getCenter().x - tmpCenter.x);
            if ((xDist) < 0) dist = -dist;
            ret.x = -dist;

        }

        else if (glm::abs(yDist) > glm::abs(zDist)) {

            dist = (getRadius() + other->getSizeY() / 2) - glm::abs(other->getCenter().y - tmpCenter.y);
            if ((yDist) < 0) dist = -dist;
            ret.y = -dist;

        }

        else {

            dist = (getRadius() + other->getSizeZ() / 2) - glm::abs(other->getCenter().z - tmpCenter.z);
            if ((zDist) < 0) dist = -dist;
            ret.z = -dist;

        }

        return ret;

    }

    //else printf("NOT COLLISION\n");

    return ret;

}

bool SphereCollider::isType(ComponentType t) {

	return t == ComponentType::SPHERECOLLIDER;

}

void SphereCollider::test() {

	printf("TEST_SPHERECOLLIDER\n");

}