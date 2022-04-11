#include "LightSource.h"

LightType LightSource::getLightType() {

	return type;
}

glm::vec3 LightSource::getDirection() {

	if(type == LightType::SPOT || type == LightType::DIRECTIONAL)
		return direction;

}

glm::vec3 LightSource::getPosition() {

	if (type == LightType::SPOT || type == LightType::POINT)
		return position;

}

glm::vec3 LightSource::getAmbient() {

	return ambient;

}

glm::vec3 LightSource::getDiffuse() {

	return diffuse;

}

glm::vec3 LightSource::getSpecular() {

	return specular;

}

float LightSource::getConstant() {

	if (type == LightType::POINT)
		return constant;

}

float LightSource::getLinear() {

	if (type == LightType::POINT)
		return linear;

}

float LightSource::getQuadratic() {

	if (type == LightType::POINT)
		return quadratic;

}

float LightSource::getCutOff() {

	if (type == LightType::DIRECTIONAL)
		return cutOff;

}

void LightSource::setDirection(glm::vec3 d) {

	if (type == LightType::SPOT || type == LightType::DIRECTIONAL)
		direction = d;

}

void LightSource::setPosition(glm::vec3 p) {

	if (type == LightType::SPOT || type == LightType::POINT)
		position = p;

}

void LightSource::setAmbient(glm::vec3 a) {

	ambient = a;

}

void LightSource::setDiffuse(glm::vec3 d) {

	diffuse = d;

}

void LightSource::setSpecular(glm::vec3 s) {

	specular = s;

}