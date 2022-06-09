#include "PickaxeAI.h"

void PickaxeAI::onCollisionEnter() {

	printf("ENTERED COLLISION\n");

};

void PickaxeAI::onTriggerEnter(BoxCollider collided) {

	//printf("TRIGGERED COLLISION\n");

}

void PickaxeAI::update(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !isThrown) {

		printf("%d/n", playerDir);
		throwTime = glfwGetTime();
		isThrown = true;
		throwDir = playerDir;
		throwFacingDir = playerFacingDir;

	}

	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && isThrown) isThrown = false;

	if (isThrown) pickaxeThrow(throwDir, throwFacingDir);

	else parent->getComponent<Model>(ComponentType::MODEL)->transform.position = playerPos;

}

void PickaxeAI::pickaxeThrow(int dir, int orientation) {

	if(orientation == 1)
		parent->getComponent<Model>(ComponentType::MODEL)->transform.position.x += orientation * dir * glm::sin(0.5 * (glfwGetTime() - throwTime));
	else
		parent->getComponent<Model>(ComponentType::MODEL)->transform.position.z += orientation * dir * glm::sin(0.5 * (glfwGetTime() - throwTime));

	parent->getComponent<Model>(ComponentType::MODEL)->transform.position.y += glm::cos(0.5*(glfwGetTime() - throwTime));

}