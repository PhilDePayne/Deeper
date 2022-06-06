#include "BoxCollider.h"
#include "LogMacros.h"

#ifdef DEEPER_BOXCOLLIDER_CLASS_LOGS

#define TEST_LOG

#endif


BoxCollider::BoxCollider(glm::vec3 center, glm::vec3 size) : center(center), size(size) {};

BoxCollider::~BoxCollider() {};

glm::vec3 BoxCollider::getCenter() {

	return center;

}

float BoxCollider::getSizeX() {

	return size.x;

}

float BoxCollider::getSizeY() {

	return size.y;

}

float BoxCollider::getSizeZ() {

	return size.z;

}

float BoxCollider::getMaxX() {

	return center.x + (size.x / 2);

};

float BoxCollider::getMaxY() {

	return center.y + (size.y / 2);

};

float BoxCollider::getMaxZ() {

	return center.z + (size.z / 2);

};

float BoxCollider::getMinX() {

	return center.x - (size.x / 2);

};

float BoxCollider::getMinY() {

	return center.y - (size.y / 2);

};

float BoxCollider::getMinZ() {

	return center.z - (size.z / 2);

};

void BoxCollider::setCenter(glm::vec3 c) {

	center = c;

}

void BoxCollider::setSize(glm::vec3 s) {

	size = s;

}

bool BoxCollider::isType(ComponentType t) {

	return t == ComponentType::BOXCOLLIDER;

}

void BoxCollider::test() {
#ifdef TEST_LOG
	printf("TEST_BOXCOLLIDER\n");
#endif
}