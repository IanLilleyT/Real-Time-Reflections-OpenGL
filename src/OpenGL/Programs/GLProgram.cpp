#include "GLProgram.h"

GLProgram::GLProgram(){}
GLProgram::~GLProgram(){}

void GLProgram::bindUniformBlock(std::string uniformBlockName)
{
	GLUniformBlockHelper* glUniformBlockHelper = Singleton<GLUniformBlockHelper>::Instance();
	GLUniformBlock* uniformBlock = glUniformBlockHelper->findUniformBlock(uniformBlockName);
	uniformBlock->bindToProgram(this->program);
}
void GLProgram::bindUniformBlocks(){}
void GLProgram::fillUniforms(){}

//Setters
void GLProgram::setProgram(GLuint program) //called from GLProgramDatabase
{
    this->program = program;
	this->bindUniformBlocks();
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