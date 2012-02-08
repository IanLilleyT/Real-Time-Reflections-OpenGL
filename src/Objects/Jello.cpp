#include "Jello.h"

std::string Jello::SHAPE_CUBE = "CUBE";
std::string Jello::SHAPE_SPHERE = "SPHERE";

Jello::Jello(){}
Jello::~Jello(){}

void Jello::update()
{
	//do other stuff first
	RenderObject::update();
}
void Jello::render()
{
	//do other stuff first
	RenderObject::render();
}
void Jello::initialize(std::string shape, glm::vec3 origin, glm::vec3 size, glm::uvec3 divisions)
{
	this->shape = shape;
	this->origin = origin;
	this->size = size;
	this->divisions = divisions;
}

std::string Jello::getShape()
{
	return this->shape;
}
glm::vec3 Jello::getOrigin()
{
	return this->origin;
}
glm::vec3 Jello::getSize()
{
	return this->size;
}
glm::uvec3 Jello::getDivisions()
{
	return this->divisions;
}