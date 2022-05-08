#include "LightSource.h"

LightSource::LightSource() {};

LightSource::~LightSource() {};

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

	if (type == LightType::SPOT || type == LightType::POINT)
		return constant;

}

float LightSource::getLinear() {

	if (type == LightType::SPOT || type == LightType::POINT)
		return linear;

}

float LightSource::getQuadratic() {

	if (type == LightType::SPOT || type == LightType::POINT)
		return quadratic;

}

float LightSource::getCutOff() {

	if (type == LightType::SPOT)
		return cutOff;

}

float LightSource::getOuterCutOff() {

	if (type == LightType::SPOT)
		return outerCutOff;

}

void LightSource::setLightType(LightType l) {

	type = l;

}

void LightSource::setDirection(glm::vec3 d) {

	if (type == LightType::SPOT || type == LightType::DIRECTIONAL) //TODO: blad jesli nie
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

void LightSource::setConstant(float c) { //TODO: if'y

	constant = c;

}

void LightSource::setLinear(float l) {

	linear = l;

}

void LightSource::setQuadratic(float q) {

	quadratic = q;

}

void LightSource::setCutOff(float c) {

	cutOff = glm::cos(glm::radians(c));
	printf("%f\n", cutOff);

}

void LightSource::setOuterCutOff(float c) {

	outerCutOff = glm::cos(glm::radians(c));

}

void LightSource::setLight(Shader shader) {

	if (type == LightType::SPOT) {

		shader.setVec3("spotLight.position", getPosition());
		shader.setVec3("spotLight.direction", getDirection());
		shader.setVec3("spotLight.ambient", getAmbient());
		shader.setVec3("spotLight.diffuse", getDiffuse());
		shader.setVec3("spotLight.specular", getSpecular());
		shader.setFloat("spotLight.constant", getConstant());
		shader.setFloat("spotLight.linear", getLinear());
		shader.setFloat("spotLight.quadratic", getQuadratic());
		shader.setFloat("spotLight.cutOff", getCutOff());
		shader.setFloat("spotLight.outerCutOff", getOuterCutOff());

	}

	if (type == LightType::DIRECTIONAL) {

		shader.setVec3("dirLight.direction", getDirection());
		shader.setVec3("dirLight.ambient", getAmbient());
		shader.setVec3("dirLight.diffuse", getDiffuse());
		shader.setVec3("dirLight.specular", getSpecular());

	}

}

bool LightSource::isType(ComponentType t) {

	return (t == ComponentType::LIGHTSOURCE);

}

void LightSource::test() {

	printf("TEST_LIGHTSOURCE\n");

}