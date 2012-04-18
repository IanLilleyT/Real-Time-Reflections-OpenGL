#pragma once

#include <vector> //For loading in lights
#include <string> //For program names

#include <glm/glm.hpp> //For loading in matrices

#include "../Material.h" //For loading in materials
#include "../Objects/Object.h" //For loading in lights

class GLState
{
public:
	GLState();
	~GLState();

	glm::mat4 modelToWorldMatrix;
	glm::mat4 worldToCameraMatrix;
	glm::mat4 cameraToClipMatrix;
	glm::mat4 shadowLightWorldToClipMatrix;
	Material* material;
	std::vector<Object*> lights;
	int effectType;

	//Reflection textures
	int colorTextureFront;
	int depthTextureFront;
	int colorTextureBack;
	int depthTextureBack;
	void setReflectionTextures(int colorFront, int depthFront, int colorBack, int depthBack);
};