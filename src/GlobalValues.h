#pragma once

#include <string>

class GlobalValues
{
public:
	GlobalValues();
	~GlobalValues();

	void setDataPath(std::string path);
	std::string getDataPath();
	std::string getShaderPath();
	std::string getMeshPath();
	std::string getWorldPath();

private:
	std::string dataPath;
	std::string shaderPath;
	std::string meshPath;
	std::string worldPath;
};