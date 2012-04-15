#include "GLUniformBlockHelper.h"

std::string GLUniformBlockHelper::TYPE_PROJECTION = "ProjectionBlock";
std::string GLUniformBlockHelper::TYPE_LIGHTS = "LightsBlock";
std::string GLUniformBlockHelper::TYPE_EFFECT_TYPE = "EffectTypeBlock";

GLUniformBlockHelper::GLUniformBlockHelper(){}
GLUniformBlockHelper::~GLUniformBlockHelper(){}

void GLUniformBlockHelper::initialize()
{
	std::vector<std::string> allUniformBlockNames = std::vector<std::string>();
	allUniformBlockNames.push_back(TYPE_PROJECTION);
	allUniformBlockNames.push_back(TYPE_LIGHTS);
	allUniformBlockNames.push_back(TYPE_EFFECT_TYPE);

	int bindingIndex = 0;
	int uniformBlockSize = 0;
	std::string uniformBlockName;
	for(unsigned int i = 0; i < allUniformBlockNames.size(); i++)
	{
		uniformBlockName = allUniformBlockNames.at(i);

		if(uniformBlockName == TYPE_PROJECTION)
			uniformBlockSize = sizeof(ProjectionBlock);
		else if(uniformBlockName == TYPE_LIGHTS)
			uniformBlockSize = sizeof(LightsBlock);
		else if(uniformBlockName == TYPE_EFFECT_TYPE)
			uniformBlockSize = sizeof(EffectTypeBlock);

		GLUniformBlock* uniformBlock = new GLUniformBlock();
		uniformBlock->setName(uniformBlockName);
		uniformBlock->setBindingIndex(bindingIndex);
		uniformBlock->setSize(uniformBlockSize);
		uniformBlock->createUBO();
		bindingIndex++;
		this->uniformBlockMap.insert(std::pair<std::string,GLUniformBlock*>(uniformBlock->getName(), uniformBlock));
	}
}

void GLUniformBlockHelper::update(std::string name)
{
	GLState* glState = Singleton<GLState>::Instance();
	GLCamera* glCamera = Singleton<GLCamera>::Instance();
	glm::mat4 worldToCameraMatrix = glCamera->getWorldToCameraMatrix();
	glm::mat4 cameraToClipMatrix = glCamera->getCameraToClipMatrix();

	if(name == TYPE_PROJECTION)
	{
		GLUniformBlock* projectionUniformBlock = this->findUniformBlock(TYPE_PROJECTION);
		ProjectionBlock projectionBlock = ProjectionBlock();
		projectionBlock.cameraToClipMatrix = cameraToClipMatrix;
		projectionBlock.zNear = glCamera->getFrustumNear();
		projectionBlock.zFar = glCamera->getFrustumFar();
		projectionUniformBlock->setData(&projectionBlock);
	}
	else if(name == TYPE_LIGHTS)
	{
		GLUniformBlock* lightsUniformBlock = this->findUniformBlock(TYPE_LIGHTS);
		LightsBlock lightsBlock = LightsBlock();
		std::vector<Object*>& lights = glState->getLights();
		int numLights = std::min((int)lights.size(), (int)MAX_NUMBER_OF_LIGHTS);
		for(int i = 0; i < numLights; i++)
		{
			Light* light = dynamic_cast<Light*>(lights.at(i));
			glm::vec4 lightPos = glm::vec4(light->getTranslation(), 1.0f);
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
	else if(name == TYPE_EFFECT_TYPE)
	{
		GLUniformBlock* effectTypeUniformBlock = this->findUniformBlock(TYPE_EFFECT_TYPE);
		EffectTypeBlock effectTypeBlock = EffectTypeBlock();
		effectTypeBlock.effectType = glState->getEffectType();
		effectTypeUniformBlock->setData(&effectTypeBlock);
	}
}
void GLUniformBlockHelper::updateAll()
{
	this->update(TYPE_PROJECTION);
	this->update(TYPE_LIGHTS);
	this->update(TYPE_EFFECT_TYPE);
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