#include "PlainMesh.h"

PlainMesh::PlainMesh() {

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	//TODO: skonczyc

}

PlainMesh::~PlainMesh() {

}

std::vector<float> PlainMesh::getVertices() {

	return vertices;

}

std::vector<int> PlainMesh::getIndices() {

	return indices;

}

unsigned int PlainMesh::getVAO() {

	return VAO;

};

unsigned int PlainMesh::getVBO() {

	return VBO;

}

unsigned int PlainMesh::getEBO() {

	return EBO;

}

bool PlainMesh::isType(ComponentType t) {

	return (t == ComponentType::PLAINMESH);

}

void PlainMesh::test() {

	printf("TEST_PLAINMESH");

}
