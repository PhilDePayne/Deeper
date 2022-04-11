#include "BoxCollider.h"

BoxCollider::BoxCollider(glm::vec3 center, glm::vec3 size) : center(center), size(size) {};

BoxCollider::~BoxCollider() {};

glm::vec3 BoxCollider::getCenter() {

	return center;

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

	printf("TEST_BOXCOLLIDER\n");

}