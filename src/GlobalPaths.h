#pragma once

#include <string> //For storing paths

class GlobalPaths
{
public:
	GlobalPaths();
	~GlobalPaths();

	void setDataPath(std::string path);
	std::string getDataPath();
	std::string getShaderPath();
	std::string getMeshPath();
	std::string getWorldPath();
	std::string getMaterialPath();

private:
	std::string dataPath;
	std::string shaderPath;
	std::string meshPath;
	std::string worldPath;
	std::string materialPath;
};