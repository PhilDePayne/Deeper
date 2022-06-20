#include "PickaxeAI.h"
#include "Sound.h"
#include "Animator.h"

void PickaxeAI::onCollisionEnter(BoxCollider collided) {

	printf("ENTERED COLLISION\n");

};

void PickaxeAI::onTriggerEnter(BoxCollider collided, Tag colliderTag) {

	//printf("TRIGGERED COLLISION\n");

}

void PickaxeAI::update(GLFWwindow* window, float deltaTime) {

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !isThrown) {

		printf("%d %d\n", playerDir, playerFacingDir);
		throwTime = glfwGetTime();
		isThrown = true;
		throwDir = playerDir;
		throwFacingDir = playerFacingDir;
		throwReverse = reverse;

        if (throwSound != nullptr)
        {
            throwSound->play();
        }

        if (throwAnimator != nullptr)
        {
            throwAnimator->play();
        }

	}

	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && isThrown) isThrown = false;

	if (isThrown) pickaxeThrow(throwDir, throwFacingDir, throwReverse, deltaTime);

	else {
		parent->getComponent<Model>(ComponentType::MODEL)->transform.position = playerPos;

		if (playerFacingDir == 1) {
			parent->getComponent<Model>(ComponentType::MODEL)->transform.position.x += reverse * playerDir * 10.0f;
			parent->getComponent<Model>(ComponentType::MODEL)->transform.position.z += reverse * playerDir * 10.0f;
		}
		else {
			parent->getComponent<Model>(ComponentType::MODEL)->transform.position.z -= reverse * playerDir * 10.0f;
			parent->getComponent<Model>(ComponentType::MODEL)->transform.position.x += reverse * playerDir * 10.0f;
		}

		parent->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(parent->getComponent<Model>(ComponentType::MODEL)->transform.position);
	}

}

void PickaxeAI::pickaxeThrow(int dir, int orientation, int reverse, float deltaTime) {

	float distance = glm::sin(1 * (glfwGetTime() - throwTime));

	if (distance > 0 || firstTravel) {
		if (orientation == 1)
			parent->getComponent<Model>(ComponentType::MODEL)->transform.position.x += reverse * dir * glm::sin(1 * (glfwGetTime() - throwTime));
		else
			parent->getComponent<Model>(ComponentType::MODEL)->transform.position.z += reverse * -dir * glm::sin(1 * (glfwGetTime() - throwTime));

		firstTravel = false;
	}

	else {
		isThrown = false;
		firstTravel = true;
		//parent->getComponent<Model>(ComponentType::MODEL)->transform.position = AI::moveTowards(parent->getComponent<Model>(ComponentType::MODEL)->transform.position, playerPos, 1 * deltaTime);
	}


	//parent->getComponent<Model>(ComponentType::MODEL)->transform.position.y += glm::cos(1*(glfwGetTime() - throwTime));

	parent->getComponent<SphereCollider>(ComponentType::SPHERECOLLIDER)->setCenter(parent->getComponent<Model>(ComponentType::MODEL)->transform.position);

}

void PickaxeAI::setThrowSound(Sound *newThrowSound)
{
    throwSound = newThrowSound;
}

void PickaxeAI::setThrowAnimator(Animator *newThrowAnimator)
{
    throwAnimator = newThrowAnimator;
}
