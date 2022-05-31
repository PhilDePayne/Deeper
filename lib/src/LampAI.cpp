#include "LampAI.h"

void LampAI::onCollisionEnter() {

	printf("ENTERED COLLISION\n");

};

void LampAI::onTriggerEnter(BoxCollider collided) {

	//printf("TRIGGERED COLLISION\n");
	bool exists = false;

	for (auto i : *lights) {

		if (i == collided.getCenter()) exists = true;

	}

	if (!exists) {

		lights->erase(lights->begin());
		//glm::vec3 newLightPos = collided.getCenter();
		lights->push_back(collided.getCenter());

	}

}
