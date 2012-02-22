#pragma once

#include <map> //For storing UniformBlocks
#include <vector> //For storage
#include <string> //For names

#include "GLUniformBlock.h" //For storing UniformBlocks
#include "GLState.h" //For getting current world values
#include "../Objects/Light.h" //For lights
#include "../Singleton.h" //For accessing GLState

class GLUniformBlockHelper
{
public:
	GLUniformBlockHelper();
	~GLUniformBlockHelper();
	void initialize();
	void update();
	GLUniformBlock* findUniformBlock(std::string name);

	static std::string TYPE_PROJECTION;
	static std::string TYPE_LIGHTS;
private:
	std::map<std::string, GLUniformBlock*> uniformBlockMap;
};

//----------------
// Structs ///////
//----------------
struct ProjectionBlock
{
	glm::mat4 cameraToClipMatrix;
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