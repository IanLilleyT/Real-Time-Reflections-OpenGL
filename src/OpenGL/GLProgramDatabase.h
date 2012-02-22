#pragma once

#include <fstream> //For shader compilation
#include <string> //For names
#include <map> //For storing programs
#include <gl3w/gl3w.h> //For OpenGL commands

#include "GLProgram.h" //For storing programs
#include "../GlobalPaths.h" //For loading programs path
#include "../Factory.h" //For creating new program classes
#include "../Singleton.h" //For loading global classes

class GLProgramDatabase
{
public:
	GLProgramDatabase();
	~GLProgramDatabase();

	GLProgram* loadProgram(std::string ProgramName);
	
	static std::string NO_NAME;

private:
	GLProgram* findProgram(std::string ProgramName);
	GLProgram* makeProgramByName(std::string name);
	std::pair<std::string, std::string> getShadersByType(std::string shaderType);
	std::map<std::string,GLProgram*> programMap;

	//Compiling shaders -- low level
	const char* GetShaderName(GLenum eShaderType);
	GLuint CreateShader(GLenum eShaderType,const std::string &strShaderFile, const std::string &strShaderName);
	GLuint LoadShader(GLenum eShaderType, const std::string &strShaderFilename);
	GLuint CreateProgram(const std::vector<GLuint> &shaderList);
};
