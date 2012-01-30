#include "GLProgramDatabase.h"

std::string GLProgramDatabase::TYPE_WHITE = "White";
std::string GLProgramDatabase::TYPE_MATERIAL = "Material";

GLProgramDatabase::GLProgramDatabase(){}
GLProgramDatabase::~GLProgramDatabase(){}

GLProgram* GLProgramDatabase::loadProgram(std::string programType)
{
	GLProgram* newProgram = 0;
	newProgram = this->findProgram(programType);
	if(newProgram == 0) 
		newProgram = this->makeProgramByName(programType);
	if(newProgram != 0)
		this->programMap[programType] = newProgram;
	return newProgram;
}
GLProgram* GLProgramDatabase::findProgram(std::string programType)
{
	std::map<std::string, GLProgram*>::iterator it;
	it = this->programMap.find(programType);
	if(it != this->programMap.end())
		return it->second;
	else
		return 0;
}
GLProgram* GLProgramDatabase::makeProgramByName(std::string name)
{
	//Given a program name, this assembles a new program object
	GLProgram* glProgram = 0;

	//CUSTOMIZE
	if(name == GLProgramDatabase::TYPE_WHITE)
		glProgram = new GLProgram_White();
	else if(name == GLProgramDatabase::TYPE_MATERIAL)
		glProgram = new GLProgram_Material();

	//Assembling the program
	if(glProgram != 0)
	{
		GlobalValues* globalValues = Singleton<GlobalValues>::Instance();
		std::pair<std::string,std::string> shaderPair = this->getShadersByType(name);
		std::string vertShader = globalValues->getShaderPath() + shaderPair.first;
		std::string fragShader = globalValues->getShaderPath() + shaderPair.second;

		std::vector<GLuint> shaderList;
		shaderList.push_back(this->LoadShader(GL_VERTEX_SHADER, vertShader));
		shaderList.push_back(this->LoadShader(GL_FRAGMENT_SHADER, fragShader));
		glProgram->setProgram(this->CreateProgram(shaderList));
		glProgram->setName(name);
	}
	return glProgram;
}

std::pair<std::string, std::string> GLProgramDatabase::getShadersByType(std::string shaderType)
{
	//Given a program name, returns programNameVert.vert and programNameFrag.frag
	std::string vertShader = shaderType + "Vert.vert";
	std::string fragShader = shaderType + "Frag.frag";
	std::pair<std::string,std::string> shaderPair(vertShader, fragShader);
	return shaderPair;
}

//All low level stuff -- DO NOT LOOK HERE
const char* GLProgramDatabase::GetShaderName(GLenum eShaderType)
{
	switch(eShaderType)
	{
		case GL_VERTEX_SHADER: return "vertex"; break;
		case GL_GEOMETRY_SHADER: return "geometry"; break;
		case GL_FRAGMENT_SHADER: return "fragment"; break;
	}
	return NULL;
}
GLuint GLProgramDatabase::LoadShader(GLenum eShaderType, const std::string &strShaderFilename)
{
	std::string strFilename = strShaderFilename;
	std::ifstream shaderFile(strFilename.c_str());
	if(!shaderFile.is_open())
	{
		fprintf(stderr, "Cannot load the shader file \"%s\" for the %s shader.\n",
			strFilename.c_str(), GetShaderName(eShaderType));
		return 0;
	}
	std::stringstream shaderData;
	shaderData << shaderFile.rdbuf();
	shaderFile.close();

	return CreateShader(eShaderType, shaderData.str(), strShaderFilename);
}
GLuint GLProgramDatabase::CreateShader(GLenum eShaderType, const std::string &strShaderFile, const std::string &strShaderName)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, (const GLchar**)&strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		fprintf(stderr, "Compile failure in %s shader named \"%s\". Error:\n%s\n",
			GetShaderName(eShaderType), strShaderName.c_str(), strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}
GLuint GLProgramDatabase::CreateProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	return program;
}