#pragma once

#include "GLProgram.h"

class GLProgram_DeferredGeometryPass: public GLProgram
{
public:
	GLProgram_DeferredGeometryPass() : GLProgram(){}
	virtual ~GLProgram_DeferredGeometryPass(){}

protected:
	virtual void bindUniformBlocks()
	{
		this->bindUniformBlock(GLUniformBlockHelper::TYPE_PROJECTION);
	}
	virtual void fillUniforms()
	{
		GLState* glState = Singleton<GLState>::Instance();

		//Color texture index
		int positionTextueUnit = glState->positionTextureUnit;
		GLuint refractiveIndexUniform = glGetUniformLocation(this->program, "refractiveIndex");
		glUniform1f(refractiveIndexUniform, refractiveIndex);
	}
};