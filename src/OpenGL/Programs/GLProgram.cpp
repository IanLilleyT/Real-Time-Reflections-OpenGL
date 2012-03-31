#include "GLProgram.h"

GLProgram::GLProgram(){}
GLProgram::~GLProgram(){}

void GLProgram::addAttribute(std::string name, GLushort size, GLenum type)
{
    GLAttribute* attribute = new GLAttribute();
    attribute->setAttributePos(glGetAttribLocation(this->program, name.c_str()));
    attribute->setSize(size);
    attribute->setType(type);
	this->attributes.push_back(attribute);
}
void GLProgram::bindUniformBlock(std::string uniformBlockName)
{
	GLUniformBlockHelper* glUniformBlockHelper = Singleton<GLUniformBlockHelper>::Instance();
	GLUniformBlock* uniformBlock = glUniformBlockHelper->findUniformBlock(uniformBlockName);
	uniformBlock->bindToProgram(this->program);
}
void GLProgram::bindUniformBlocks(){}
void GLProgram::fillUniforms(){}
void GLProgram::createAttributes(){}

//Setters
void GLProgram::setProgram(GLuint program) //called from GLProgramDatabase
{
    this->program = program;
	this->bindUniformBlocks();
	this->createAttributes();
}
void GLProgram::setName(std::string name) //called from GLProgramDatabase
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
std::vector<GLAttribute*>& GLProgram::getAttributes()
{
	return this->attributes;
}