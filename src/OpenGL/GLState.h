#pragma once

#include <string>

#include <glm/glm.hpp> //For loading in matrices

#include "../Material.h"

class GLState
{
public:
	GLState(){}
	~GLState(){}
	glm::mat4 modelToWorldMatrix;
	glm::mat4 worldToCameraMatrix;
	glm::mat4 cameraToClipMatrix;
	Material* material;
	std::string globalProgramName;
};