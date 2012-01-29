#pragma once

#include <vector>
#include "glm/glm.hpp"

class Face
{
public:
    Face();
    ~Face();

	void addVertexAndNormal(glm::vec3 vertex, glm::vec3 normal);
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec3> getNormals();
	glm::vec3 getFaceNormal();
	
private:
	void calculateNormal();
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::vec3 faceNormal;

	const static int MAX_VERTICES = 3;
};
