#include "GLUniformBlockHelper.h"

std::string GLUniformBlockHelper::TYPE_PROJECTION = "ProjectionBlock";
std::string GLUniformBlockHelper::TYPE_LIGHT = "LightBlock";

GLUniformBlockHelper::GLUniformBlockHelper(){}
GLUniformBlockHelper::~GLUniformBlockHelper(){}

void GLUniformBlockHelper::initialize()
{
	std::vector<std::string> allUniformBlockNames = std::vector<std::string>();
	allUniformBlockNames.push_back(TYPE_PROJECTION);
	allUniformBlockNames.push_back(TYPE_LIGHT);

	int bindingIndex = 0;
	int uniformBlockSize = 0;
	std::string uniformBlockName;
	for(unsigned int i = 0; i < allUniformBlockNames.size(); i++)
	{
		uniformBlockName = allUniformBlockNames.at(i);

		if(uniformBlockName == TYPE_PROJECTION)
			uniformBlockSize = sizeof(ProjectionBlock);
		else if(uniformBlockName == TYPE_LIGHT)
			uniformBlockSize = sizeof(LightBlock);

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
	GLView* glView = Singleton<GLView>::Instance();

	if(name == TYPE_PROJECTION)
	{
		GLUniformBlock* projectionUniformBlock = this->findUniformBlock(TYPE_PROJECTION);
		ProjectionBlock projectionBlock = ProjectionBlock();
		projectionBlock.cameraToClipMatrix = glState->cameraToClipMatrix;
		projectionBlock.zNear = glView->getFrustumNear();
		projectionBlock.zFar = glView->getFrustumFar();
		projectionBlock.screenWidth = (float)glView->getWindowDimensions().x;
		projectionBlock.screenHeight = (float)glView->getWindowDimensions().y;
		projectionUniformBlock->setData(&projectionBlock);
	}
	else if(name == TYPE_LIGHT)
	{
		GLUniformBlock* lightUniformBlock = this->findUniformBlock(TYPE_LIGHT);
		LightBlock lightBlock = LightBlock();
		lightBlock.ambientIntensity = Light::getAmbientIntensity();
		lightBlock.gamma = Light::getGamma();
		lightBlock.lightAttenuation = Light::getLightAttenuation();
		lightBlock.maxIntensity = Light::getMaxIntensity();
		lightUniformBlock->setData(&lightBlock);
	}
}
void GLUniformBlockHelper::updateAll()
{
	this->update(TYPE_PROJECTION);
	this->update(TYPE_LIGHT);
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