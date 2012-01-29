#include "Face.h"

Face::Face(){}
Face::~Face(){}

void Face::addVertexAndNormal(glm::vec3 vertex, glm::vec3 normal)
{
	//Add vertex and normal
	if(this->vertices.size() < Face::MAX_VERTICES)
	{
		this->vertices.push_back(vertex);
		this->normals.push_back(normal);
	}

	//Calculate normal if a triangle
	if(this->vertices.size() == MAX_VERTICES)
		this->calculateNormal();
}
void Face::calculateNormal()
{
	glm::vec3 vector1 = this->vertices.at(1) - this->vertices.at(0);
	glm::vec3 vector2 = this->vertices.at(2) - this->vertices.at(0);
	this->faceNormal = glm::normalize(glm::cross(vector1,vector2));
}
glm::vec3 Face::getFaceNormal()
{
	return this->faceNormal;
}
std::vector<glm::vec3> Face::getVertices()
{
	return this->vertices;
}
std::vector<glm::vec3> Face::getNormals()
{
	return this->normals;
}
