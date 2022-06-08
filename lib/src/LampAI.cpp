#include "LampAI.h"
#include "LogMacros.h"

#ifdef DEEPER_LAMPAI_CLASS_LOGS

#define ON_COLLISION_ENTER_LOG

#endif


void LampAI::onCollisionEnter(BoxCollider collided) {
#ifdef ON_COLLISION_ENTER_LOG
	printf("ENTERED COLLISION\n");
#endif
}

void LampAI::onTriggerEnter(BoxCollider collided) {

	//printf("TRIGGERED COLLISION\n");
	bool exists = false;

	for (auto i : *lights) {

		if (i == collided.getCenter()) exists = true;

	}

	if (!exists) {

		lights->erase(lights->begin());
		lights->push_back(collided.getCenter());

	}

}

void LampAI::update(GLFWwindow* window, float deltaTime) {

}
