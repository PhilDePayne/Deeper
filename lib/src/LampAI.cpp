#include "LampAI.h"
#include "Sound.h"
#include "LogMacros.h"

#ifdef DEEPER_LAMPAI_CLASS_LOGS

#define ON_COLLISION_ENTER_LOG
#define SOUND_LOG

#endif


void LampAI::onCollisionEnter(BoxCollider collided) {
#ifdef ON_COLLISION_ENTER_LOG
	printf("ENTERED COLLISION\n");
#endif
}

void LampAI::onTriggerEnter(BoxCollider collided, Tag colliderTag) {

	bool exists = false;
	int l = 0;

	for (auto i : *lights) {

		if (i == collided.getCenter()) {
		    exists = true;

		    if(!lit->at(l))
		        lit->at(l) = true;
		}
        l++;
	}

	if (colliderTag == Tag::LARVA && exists) {

		lights->erase(std::remove(lights->begin(), lights->end(), collided.getCenter()), lights->end());
		lights->push_back(glm::vec3(1000.0f)); //TODO: sprawdzic czy zawsze bedzie dzialalo
		//eaten = true;
		lit->pop_back(); //do sprawdzenia

	}

	else if (!exists && colliderTag != Tag::LARVA) {

		lights->erase(lights->begin());
		lights->push_back(collided.getCenter());
		printf("TURN ON\n");

#ifdef DEEPER_LAMPAI_CLASS_LOGS
        std::cout << "DOESN'T EXIST\n";
#endif

		//idk
		lit->push_back(true);

        if (turnOnSound != nullptr)
        {
#ifdef SOUND_LOG
            printf("TurnLamp sound played!");
#endif
            turnOnSound->play();
        }
#ifdef SOUND_LOG
        else
        {
            printf("TurnLamp sound NOT played!");
        }
#endif
	}

}

void LampAI::update(GLFWwindow* window, float deltaTime) {

}

void LampAI::setTurnOnSound(Sound *newTurnOnSound)
{
    turnOnSound = newTurnOnSound;
}
