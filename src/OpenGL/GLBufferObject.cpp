#include "GLBufferObject.h"

GLBufferObject::GLBufferObject()
{
	this->vertexBuffer = 0;
	this->indexBuffer = 0;
}
GLBufferObject::~GLBufferObject()
{
	//Do nothing
}

//----------------
// Setters ///////
//----------------
void GLBufferObject::setVertexBuffer(GLuint vertexBuffer)
{
	this->vertexBuffer = vertexBuffer;
}
void GLBufferObject::setIndexBuffer(GLuint indexBuffer)
{
	this->indexBuffer = indexBuffer;
}
void GLBufferObject::setDrawType(GLenum drawType)
{
	this->drawType = drawType;
}
void GLBufferObject::setVertexBufferData(std::vector<GLfloat> vertexBufferData)
{
	this->vertexBufferData = vertexBufferData;
}
void GLBufferObject::setIndexBufferData(std::vector<GLushort> indexBufferData)
{
	this->indexBufferData = indexBufferData;
}
void GLBufferObject::setNumElements(GLuint numElements)
{
	this->numElements = numElements;
}

//----------------
// Getters ///////
//----------------
GLuint GLBufferObject::getVertexBuffer()
{
	return this->vertexBuffer;
}
GLuint GLBufferObject::getIndexBuffer()
{
	return this->indexBuffer;
}
GLenum GLBufferObject::getDrawType()
{
	return this->drawType;
}
std::vector<GLfloat>& GLBufferObject::getVertexBufferData()
{
	return this->vertexBufferData;
}
std::vector<GLushort>& GLBufferObject::getIndexBufferData()
{
	return this->indexBufferData;
}
GLuint GLBufferObject::getNumElements()
{
	return this->numElements;
}