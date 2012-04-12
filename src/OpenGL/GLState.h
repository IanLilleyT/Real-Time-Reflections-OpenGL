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

	void setModelToWorldMatrix(glm::mat4 modelToWorldMatrix);
	glm::mat4 getModelToWorldMatrix();

	void setMaterial(Material* material);
	Material* getMaterial();

	void setLights(std::vector<Object*>& lights);
	std::vector<Object*>& getLights();

	void setReflectionToggle(int reflectionToggle);
	int getReflectionToggle();

private:
	glm::mat4 modelToWorldMatrix;
	glm::mat4 worldToCameraMatrix;
	glm::mat4 cameraToClipMatrix;
	Material* material;
	std::vector<Object*> lights;
	int reflectionToggle;
};