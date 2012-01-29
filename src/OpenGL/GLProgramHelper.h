#pragma once

#include <string>
#include <map>
#include <gl3w/gl3w.h>

#include "GLProgram.h"
#include "Programs/GLProgram_Material.h"
#include "Programs/GLProgram_White.h"
#include "../GlobalValues.h"

class GLProgramHelper
{
public:
	GLProgramHelper();
	~GLProgramHelper();
	void initialize();
	GLProgram* findProgram(std::string name);
	
	static std::string TYPE_WHITE;
	static std::string TYPE_MATERIAL;
	static std::string TYPE_DEFAULT_MESH;

private:

	std::map<std::string,GLProgram*> programMap;
	GLProgram* makeProgramByName(std::string name);
	std::pair<std::string, std::string> getShadersByType(std::string shaderType);

	//Compiling shaders -- low level
	const char* GetShaderName(GLenum eShaderType);
	GLuint CreateShader(GLenum eShaderType,const std::string &strShaderFile, const std::string &strShaderName);
	GLuint LoadShader(GLenum eShaderType, const std::string &strShaderFilename);
	GLuint CreateProgram(const std::vector<GLuint> &shaderList);
};
