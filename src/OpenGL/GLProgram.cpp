#include "GLProgram.h"

GLProgram::GLProgram()
{
	this->uniformBlocks = std::vector<std::string>();
}
GLProgram::~GLProgram(){}

GLAttribute* GLProgram::getAttribute(std::string name, GLushort size, GLenum type)
{
    GLAttribute* attribute = new GLAttribute();
    attribute->setAttributePos(glGetAttribLocation(this->program, name.c_str()));
    attribute->setSize(size);
    attribute->setType(type);
    return attribute;
}
void GLProgram::setProgram(GLuint program)
{
    this->program = program;
	this->bindUniformBlocks();
}
void GLProgram::setName(std::string name)
{
    this->name = name;
}

//Getters
GLuint GLProgram::getProgram()
{
    return this->program;
}
std::string GLProgram::getName()
{
    return this->name;
}
void GLProgram::fillUniforms(){}
void GLProgram::bindUniformBlocks()
{
	GLUniformBlockHelper* glUniformBlockHelper = Singleton<GLUniformBlockHelper>::Instance();
	for(unsigned int i = 0; i < this->uniformBlocks.size(); i++)
	{
		std::string uniformBlockName = this->uniformBlocks.at(i);
		GLUniformBlock* uniformBlock = glUniformBlockHelper->findUniformBlock(uniformBlockName);
		uniformBlock->bindToProgram(this->program);
	}
}