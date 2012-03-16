#include "GLProgram.h"

GLProgram::GLProgram(){}
GLProgram::~GLProgram(){}

GLAttribute* GLProgram::getAttribute(std::string name, GLushort size, GLenum type)
{
    GLAttribute* attribute = new GLAttribute();
    attribute->setAttributePos(glGetAttribLocation(this->program, name.c_str()));
    attribute->setSize(size);
    attribute->setType(type);
    return attribute;
}
void GLProgram::bindUniformBlock(std::string uniformBlockName)
{
	GLUniformBlockHelper* glUniformBlockHelper = Singleton<GLUniformBlockHelper>::Instance();
	GLUniformBlock* uniformBlock = glUniformBlockHelper->findUniformBlock(uniformBlockName);
	uniformBlock->bindToProgram(this->program);
}
void GLProgram::bindUniformBlocks(){}
void GLProgram::fillUniforms(){}
void GLProgram::createVAO(){}

//Setters
void GLProgram::setProgram(GLuint program) //called from GLProgramDatabase
{
    this->program = program;
	this->bindUniformBlocks();
	this->vao = new GLVertexArrayObject();
	this->createVAO();
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
GLVertexArrayObject* GLProgram::getVAO()
{
	return this->vao;
}