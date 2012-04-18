#include "GLState.h"

GLState::GLState(){};
GLState::~GLState(){};

//Reflection textures
void GLState::setReflectionTextures(int colorFront, int depthFront, int colorBack, int depthBack)
{
	this->colorTextureFront = colorFront;
	this->depthTextureFront = depthFront;
	this->colorTextureBack = colorBack;
	this->depthTextureBack = depthBack;
}