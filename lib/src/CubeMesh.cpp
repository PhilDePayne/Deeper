#include "CubeMesh.h"

CubeMesh::CubeMesh() {

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

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
