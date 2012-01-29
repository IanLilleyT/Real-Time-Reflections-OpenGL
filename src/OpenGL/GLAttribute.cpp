#include "GLAttribute.h"

GLAttribute::GLAttribute()
{
    //Do nothing
}
GLAttribute::~GLAttribute()
{
    //Do nothing
}

//Setters
void GLAttribute::setAttributePos(GLuint attributePos)
{
    this->attributePos = attributePos;
}
void GLAttribute::setSize(GLushort size)
{
    this->size = size;
}
void GLAttribute::setType(GLenum type)
{
    this->type = type;
}

//Getters
GLuint GLAttribute::getAttributePos()
{
    return this->attributePos;
}
GLushort GLAttribute::getSize()
{
    return this->size;
}
GLenum GLAttribute::getType()
{
    return this->type;
}
