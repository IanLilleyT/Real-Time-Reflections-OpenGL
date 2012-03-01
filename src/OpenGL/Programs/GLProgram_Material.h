#pragma once

#include "../GLProgram.h"

class GLProgram_Material: public GLProgram
{
public:
	GLProgram_Material();
	virtual ~GLProgram_Material();
	virtual void createVAO();
protected:
	virtual void bindUniformBlocks();
	virtual void fillUniforms();
};