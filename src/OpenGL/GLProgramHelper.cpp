#include "GLProgramHelper.h"

std::string GLProgramHelper::TYPE_WHITE = "White";
std::string GLProgramHelper::TYPE_MATERIAL = "Material";
std::string GLProgramHelper::TYPE_DEFAULT_MESH = GLProgramHelper::TYPE_MATERIAL;

GLProgramHelper::GLProgramHelper(){}
GLProgramHelper::~GLProgramHelper(){}

void GLProgramHelper::initialize()
{
	std::vector<std::string> allProgramNames = std::vector<std::string>();
	allProgramNames.push_back(TYPE_WHITE);
	allProgramNames.push_back(TYPE_MATERIAL);
	
	for(unsigned int i = 0; i < allProgramNames.size(); i++)
	{
		std::string programName = allProgramNames.at(i);
		GLProgram* program = this->makeProgramByName(programName);
		this->programMap.insert(std::pair<std::string, GLProgram*>(program->getName(),program));
	}
}
GLProgram* GLProgramHelper::makeProgramByName(std::string name)
{
	//Given a program name, this assembles a new program object
	GLProgram* glProgram = 0;

	//CUSTOMIZE
	if(name == GLProgramHelper::TYPE_WHITE)
		glProgram = new GLProgram_White();
	else if(name == GLProgramHelper::TYPE_MATERIAL)
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

std::pair<std::string, std::string> GLProgramHelper::getShadersByType(std::string shaderType)
{
	//Given a program name, returns programNameVert.vert and programNameFrag.frag
	std::string vertShader = shaderType + "Vert.vert";
	std::string fragShader = shaderType + "Frag.frag";
	std::pair<std::string,std::string> shaderPair(vertShader, fragShader);
	return shaderPair;
}

GLProgram* GLProgramHelper::findProgram(std::string name)
{	
	//Returns a GLProgram object if found
	std::map<std::string, GLProgram*>::iterator iter = this->programMap.find(name);
	if(iter != this->programMap.end())
	{
		GLProgram* program = iter->second;
		return program;
	}
	return 0;
}

//All low level stuff -- DO NOT LOOK HERE
const char* GLProgramHelper::GetShaderName(GLenum eShaderType)
{
	switch(eShaderType)
	{
		case GL_VERTEX_SHADER: return "vertex"; break;
		case GL_GEOMETRY_SHADER: return "geometry"; break;
		case GL_FRAGMENT_SHADER: return "fragment"; break;
	}
	return NULL;
}
GLuint GLProgramHelper::LoadShader(GLenum eShaderType, const std::string &strShaderFilename)
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
GLuint GLProgramHelper::CreateShader(GLenum eShaderType, const std::string &strShaderFile, const std::string &strShaderName)
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
GLuint GLProgramHelper::CreateProgram(const std::vector<GLuint> &shaderList)
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