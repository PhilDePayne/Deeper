#include "SphereCollider.h"

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

bool SphereCollider::isType(ComponentType t) {

	return t == ComponentType::SPHERECOLLIDER;

}

void SphereCollider::test() {

	printf("TEST_SPHERECOLLIDER\n");

}