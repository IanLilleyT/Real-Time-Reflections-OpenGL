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

	int positionTextureUnit;
	int normalTextureUnit;
	int diffuseColorTextureUnit;
	int specularColorTextureUnit;
	int otherTextureUnit;
	int depthTextureUnit;
	int colorBufferTextureUnit;

	glm::vec3 lightCameraSpacePosition;
	glm::vec3 lightIntensity;
};