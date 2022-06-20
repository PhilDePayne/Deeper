#pragma once

#include "AI.h"
#include "Model.h"
#include "typedefs.h"

class Sound;
class Animator;

class PickaxeAI : public AI {

public:
	PickaxeAI() {};
	PickaxeAI(gameObjectPtr parent) : AI(parent) {};
	~PickaxeAI() {};

	void onCollisionEnter(BoxCollider collided) override;
	void onTriggerEnter(BoxCollider collided, Tag colliderTag) override;
	void update(GLFWwindow* window, float deltaTime) override; //TODO: override
	void onKeyPressed(int key, int action); //TODO: override
	void setThrowSound(Sound *newThrowSound);
    void setThrowAnimator(Animator *newThrowAnimator);

	glm::vec3 playerPos;
	int playerDir;
	int throwDir;
	int playerFacingDir;
	int throwFacingDir;
	int reverse;
	int throwReverse;

	bool isThrown = false;

private:
	
	double throwTime;
	Sound *throwSound = nullptr;
    Animator *throwAnimator = nullptr;
	bool firstTravel = true;

    void pickaxeThrow(int dir, int orientation, int reverse, float deltaTime);
};