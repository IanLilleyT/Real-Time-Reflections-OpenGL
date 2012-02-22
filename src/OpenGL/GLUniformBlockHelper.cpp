#include "GLUniformBlockHelper.h"

std::string GLUniformBlockHelper::TYPE_PROJECTION = "ProjectionBlock";
std::string GLUniformBlockHelper::TYPE_LIGHTS = "LightsBlock";

GLUniformBlockHelper::GLUniformBlockHelper(){}
GLUniformBlockHelper::~GLUniformBlockHelper(){}

void GLUniformBlockHelper::initialize()
{
	std::vector<std::string> allUniformBlockNames = std::vector<std::string>();
	allUniformBlockNames.push_back(TYPE_PROJECTION);
	allUniformBlockNames.push_back(TYPE_LIGHTS);

	int bindingIndex = 0;
	std::string uniformBlockName;
	int uniformBlockSize = 0;
	GLUniformBlock* uniformBlock;

	for(unsigned int i = 0; i < allUniformBlockNames.size(); i++)
	{
		uniformBlockName = allUniformBlockNames.at(i);

		if(uniformBlockName == TYPE_PROJECTION)
			uniformBlockSize = sizeof(ProjectionBlock);
		else if(uniformBlockName == TYPE_LIGHTS)
			uniformBlockSize = sizeof(LightsBlock);

		uniformBlock = new GLUniformBlock();
		uniformBlock->setName(uniformBlockName);
		uniformBlock->setBindingIndex(bindingIndex);
		uniformBlock->setSize(uniformBlockSize);
		uniformBlock->createUBO();
		bindingIndex++;
		this->uniformBlockMap.insert(std::pair<std::string,GLUniformBlock*>(uniformBlock->getName(), uniformBlock));
	}
}

void GLUniformBlockHelper::update()
{
	GLState* glState = Singleton<GLState>::Instance();
	glm::mat4 worldToCameraMatrix = glState->getWorldToCameraMatrix();
	glm::mat4 cameraToClipMatrix = glState->getCameraToClipMatrix();
	
	//Projection Block
	GLUniformBlock* projectionUniformBlock = this->findUniformBlock(TYPE_PROJECTION);
	ProjectionBlock projectionBlock = ProjectionBlock();
	projectionBlock.cameraToClipMatrix = cameraToClipMatrix;
	projectionUniformBlock->setData(&projectionBlock);

	//Lights Block
	GLUniformBlock* lightsUniformBlock = this->findUniformBlock(TYPE_LIGHTS);
	LightsBlock lightsBlock = LightsBlock();
	std::vector<Object*>& lights = glState->getLights();
	int numLights = std::min((int)lights.size(), (int)MAX_NUMBER_OF_LIGHTS);
	for(int i = 0; i < numLights; i++)
	{
		Light* light = dynamic_cast<Light*>(lights.at(i));
		glm::vec4 lightPos = glm::vec4(light->getPosition(), 1.0f);
		glm::vec4 lightPosCameraSpace = glm::vec4(glm::vec3(worldToCameraMatrix * lightPos),1.0f);
		lightsBlock.lights[i].cameraSpaceLightPos = lightPosCameraSpace;
		lightsBlock.lights[i].lightIntensity = light->getIntensity();
	}
	lightsBlock.ambientIntensity = Light::getAmbientIntensity();
	lightsBlock.gamma = Light::getGamma();
	lightsBlock.lightAttenuation = Light::getLightAttenuation();
	lightsBlock.maxIntensity = Light::getMaxIntensity();
	lightsUniformBlock->setData(&lightsBlock);
}

GLUniformBlock* GLUniformBlockHelper::findUniformBlock(std::string name)
{	
	//Returns a GLUniformBlock object if found
	std::map<std::string, GLUniformBlock*>::iterator iter = this->uniformBlockMap.find(name);
	if(iter != this->uniformBlockMap.end())
	{
		GLUniformBlock* uniformBlock = iter->second;
		return uniformBlock;
	}
	return 0;
}