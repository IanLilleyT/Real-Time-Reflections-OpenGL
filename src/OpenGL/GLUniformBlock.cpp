#include "GLUniformBlock.h"

GLUniformBlock::GLUniformBlock()
{
	//Do nothing
}
GLUniformBlock::~GLUniformBlock()
{
	//Do nothing
}

//Other
void GLUniformBlock::bindToProgram(GLuint program)
{
	GLuint uniformBlockIndex = glGetUniformBlockIndex(program, this->name.c_str());
	glUniformBlockBinding(program, uniformBlockIndex, this->bindingIndex);
}
//Setters
void GLUniformBlock::setName(std::string name)
{
	this->name = name;
}
void GLUniformBlock::createUBO()
{
	//Make sure to set the binding index before calling this!
	//Make sure to set the size before calling this!
	//(Done in the GLUniformHelper)
	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, this->size, 0, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, this->bindingIndex, ubo, 0, this->size);
	this->setUBO(ubo);
}
void GLUniformBlock::setUBO(GLuint ubo)
{
	this->ubo = ubo;
}
void GLUniformBlock::setBindingIndex(int index)
{
	this->bindingIndex = index;
}
void GLUniformBlock::setSize(int size)
{
	this->size = size;
}
void GLUniformBlock::setData(void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);	
	glBufferSubData(GL_UNIFORM_BUFFER, 0, this->size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

//Getters
std::string GLUniformBlock::getName()
{
	return this->name;
}
GLuint GLUniformBlock::getUBO()
{
	return this->ubo;
}
int GLUniformBlock::getBindingIndex()
{
	return this->bindingIndex;
}
int GLUniformBlock::getSize()
{
	return this->size;
}