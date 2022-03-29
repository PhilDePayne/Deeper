#include "CubeMesh.h"

CubeMesh::CubeMesh() {

	

};

CubeMesh::~CubeMesh() {};

unsigned int CubeMesh::getVAO() {

	return VAO;

};

unsigned int CubeMesh::getVBO() {

	return VBO;

}

std::vector<float> CubeMesh::getVertices() {

	return vertices;

}

bool CubeMesh::isType(ComponentType t) {

	return (t == ComponentType::CUBEMESH);

}

void CubeMesh::test() {

	printf("TEST_CUBEMESH\n");

}
