#pragma once

#include <map> //For storing UniformBlocks
#include <vector> //For storage
#include <string> //For names

#include "GLUniformBlock.h" //For storing UniformBlocks
#include "GLState.h" //For getting current world values
#include "GLCamera.h" //For camera values
#include "../Objects/Light.h" //For lights
#include "../Utils/Singleton.h" //For accessing GLState

class GLUniformBlockHelper
{
public:
	GLUniformBlockHelper();
	~GLUniformBlockHelper();

	void initialize();
	void updateAll();
	void update(std::string name);
	GLUniformBlock* findUniformBlock(std::string name);

	static std::string TYPE_PROJECTION;
	static std::string TYPE_LIGHTS;
	static std::string TYPE_EFFECT_TYPE;

	static const int DIFFUSE = 0;
	static const int REFLECTION = 1;
	static const int REFRACTION = 2;

private:
	std::map<std::string, GLUniformBlock*> uniformBlockMap;
};

//----------------
// Structs ///////
//----------------
struct ProjectionBlock
{
	glm::mat4 cameraToClipMatrix;
	float zNear;
	float zFar;
};
struct PerLight
{
	glm::vec4 cameraSpaceLightPos;
	glm::vec4 lightIntensity;
};
const int MAX_NUMBER_OF_LIGHTS = 8;
struct LightsBlock
{
	glm::vec4 ambientIntensity;
	float lightAttenuation;
	float maxIntensity;
	float gamma;
	float padding;
	PerLight lights[MAX_NUMBER_OF_LIGHTS];
};
struct EffectTypeBlock
{
	int effectType;
};