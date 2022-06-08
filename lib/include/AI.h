#pragma once

#include "Component.h"
#include "Shader.h"
#include "GameObject.h"
#include "Model.h"
#include "typedefs.h"

#include <GLFW/glfw3.h>

class AI : public Component {

public: 

	AI() {};
	AI(gameObjectPtr parent) : Component(parent) {};
	~AI() {};

	bool isType(ComponentType t) override;
	void test() override;

	virtual void onCollisionEnter(BoxCollider collided);
	virtual void onTriggerEnter(BoxCollider collided);
	virtual void update(GLFWwindow* window, float deltaTime);

	glm::vec3 moveTowards(glm::vec3 from, glm::vec3 to, float step);

};