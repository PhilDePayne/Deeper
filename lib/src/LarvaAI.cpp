#include "LarvaAI.h"

void LarvaAI::onCollisionEnter() {

}

void LarvaAI::onTriggerEnter(BoxCollider collided) {

}

void LarvaAI::update(GLFWwindow* window, float deltaTime) {

	glm::vec3 currentPos = parent->getComponent<Model>(ComponentType::MODEL)->transform.position;

	glm::vec3 deltaPos = moveTowards(currentPos, lights->at(0), speed * deltaTime);

	printf("Target pos: %f %f %f\n", lights->at(0).x, lights->at(0).y, lights->at(0).z);
	printf("Current pos: %f %f %f\n", currentPos.x, currentPos.y, currentPos.z);
	printf("Delta pos: %f %f %f\n", deltaPos.x, deltaPos.y, deltaPos.z);
	printf("Step vars: %f %f %f\n", speed, deltaTime, speed * deltaTime);


	parent->getComponent<Model>(ComponentType::MODEL)->transform.position = deltaPos;
	parent->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(deltaPos);

}