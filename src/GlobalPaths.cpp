#include "GlobalPaths.h"

GlobalPaths::GlobalPaths(){}
GlobalPaths::~GlobalPaths(){}

void GlobalPaths::setDataPath(std::string path)
{
	this->dataPath = path;
	this->shaderPath = dataPath + "shaders/";
	this->meshPath = dataPath + "meshes/";
	this->worldPath = dataPath + "worlds/";
	this->materialPath = dataPath + "materials/";
}
std::string GlobalPaths::getDataPath()
{
	return this->dataPath;
}
std::string GlobalPaths::getShaderPath()
{
	return this->shaderPath;
}
std::string GlobalPaths::getMeshPath()
{
	return this->meshPath;
}
std::string GlobalPaths::getWorldPath()
{
	return this->worldPath;
}
std::string GlobalPaths::getMaterialPath()
{
	return this->materialPath;
}