#include "GLVertexArrayObject.h"

GLVertexArrayObject::GLVertexArrayObject()
{
	this->vertexArrayObject = 0;
	this->attributes = std::vector<GLAttribute*>();
}
GLVertexArrayObject::~GLVertexArrayObject(){}

//Setters
void GLVertexArrayObject::setVertexArrayObject(GLuint vertexArrayObject)
{
	this->vertexArrayObject = vertexArrayObject;
}

//Getters
GLuint GLVertexArrayObject::getVertexArrayObject()
{
	return this->vertexArrayObject;
}
std::vector<GLAttribute*> GLVertexArrayObject::getAttributes()
{
	return this->attributes;
}
void GLVertexArrayObject::addAttribute(GLAttribute* attribute)
{
	this->attributes.push_back(attribute);
}
void GLVertexArrayObject::setAttributes(std::vector<GLAttribute*> attributes)
{
	this->attributes = attributes;
}