#pragma once

#include "Component.h"

#include <glm/glm.hpp>

enum class LightType {

	DIRECTIONAL,
	POINT,
	SPOT

};

class LightSource : public Component {

public:

	LightSource();
	LightSource(gameObjectPtr parent) : Component(parent) {};
	~LightSource();

	LightType getLightType();
	glm::vec3 getDirection();
	glm::vec3 getPosition();
	glm::vec3 getAmbient();
	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();
	float getConstant();
	float getLinear();
	float getQuadratic();
	float getCutOff();
	float getOuterCutOff();

	void setLightType(LightType t);
	void setDirection(glm::vec3 d);
	void setPosition(glm::vec3 p);
	void setAmbient(glm::vec3 a);
	void setDiffuse(glm::vec3 d);
	void setSpecular(glm::vec3 s);
	void setConstant(float c);
	void setLinear(float l);
	void setQuadratic(float q);
	void setCutOff(float c);
	void setOuterCutOff(float c);

	bool isType(ComponentType t);
	void test();

private:

	LightType type;

	glm::vec3 direction; //DIRECTIONAL / SPOT
	glm::vec3 position; //POINT / SPOT

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	//DIRECTIONAL / SPOT
	float constant;
	float linear;
	float quadratic;

	//SPOT
	float cutOff;
	float outerCutOff;
};