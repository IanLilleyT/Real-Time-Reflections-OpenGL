#pragma once

#include "GLFrameBuffer.h"

class GLFrameBuffer_Reflection : public GLFrameBuffer
{
	GLFrameBuffer_Reflection() : GLFrameBuffer(){}
	virtual ~GLFrameBuffer_Reflection(){}

	virtual void initialize()
	{
		GLFrameBuffer::initialize();
	}
};