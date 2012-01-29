#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../Material.h"
#include "../Objects/Light.h"

class GLState
{
public:
	GLState();
	~GLState();

	void setModelToWorldMatrix(glm::mat4 modelToWorldMatrix);
	glm::mat4 getModelToWorldMatrix();

	void setWorldToCameraMatrix(glm::mat4 worldToCameraMatrix);
	glm::mat4 getWorldToCameraMatrix();

	void setCameraToClipMatrix(glm::mat4 cameraToClipMatrix);
	glm::mat4 getCameraToClipMatrix();

	void setMaterial(Material* material);
	Material* getMaterial();

	void setLights(std::vector<Light*> lights);
	std::vector<Light*> getLights();

private:
	glm::mat4 modelToWorldMatrix;
	glm::mat4 worldToCameraMatrix;
	glm::mat4 cameraToClipMatrix;
	Material* material;
	std::vector<Light*> lights;
};