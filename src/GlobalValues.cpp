#include "GlobalValues.h"

GlobalValues::GlobalValues(){}
GlobalValues::~GlobalValues(){}

void GlobalValues::setDataPath(std::string path)
{
	this->dataPath = path;
	this->shaderPath = dataPath + "shader/";
	this->meshPath = dataPath + "mesh/";
	this->worldPath = dataPath + "world/";
	this->materialPath = dataPath + "material/";
}
std::string GlobalValues::getDataPath()
{
	return this->dataPath;
}
std::string GlobalValues::getShaderPath()
{
	return this->shaderPath;
}
std::string GlobalValues::getMeshPath()
{
	return this->meshPath;
}
std::string GlobalValues::getWorldPath()
{
	return this->worldPath;
}
std::string GlobalValues::getMaterialPath()
{
	return this->materialPath;
}