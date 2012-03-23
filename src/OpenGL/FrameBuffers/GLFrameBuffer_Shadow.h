#pragma once

#include "GLFrameBuffer.h"

class GLFrameBuffer_Shadow : public GLFrameBuffer
{
	GLFrameBuffer_Shadow() : GLFrameBuffer(){}
	virtual ~GLFrameBuffer_Shadow(){}

	virtual void initialize()
	{
		GLFrameBuffer::initialize();
	}
};