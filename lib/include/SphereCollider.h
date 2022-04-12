#pragma once

#include "Component.h"

#include <glm/glm.hpp>

class SphereCollider : public Component {

public:

	SphereCollider() {};
	SphereCollider(glm::vec3 center, float radius);
	~SphereCollider();

	glm::vec3 getCenter();
	float getRadius();

	void setCenter(glm::vec3 c);
	void setRadius(float r);

	bool isType(ComponentType t) override;
	void test() override;

private:

	glm::vec3 center;
	float radius;

};