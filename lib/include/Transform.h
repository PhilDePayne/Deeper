#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

class Transform : public Component {

public:

	Transform();
	~Transform();

	glm::mat4 getCombinedMatrix();

	glm::mat4 worldMatrix;

	glm::vec3 position = glm::vec3(0.0f);
	float z_rotation_angle = 0.0f;
	float y_rotation_angle = 0.0f;
	float x_rotation_angle = 0.0f;
	float scale = 1.0f;

	bool isType(ComponentType t) override;

	void test() override;

};