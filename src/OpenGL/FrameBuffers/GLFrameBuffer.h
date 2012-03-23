#pragma once

#include <glm/glm.hpp>
#include <gl3w/gl3w.h>

class GLFrameBuffer
{
public:
	GLFrameBuffer();
	virtual ~GLFrameBuffer();

	virtual void initialize();
	void bindForWriting();
	void bindForReading(GLenum textureUnit);

private:
	GLuint fbo;
	GLuint texture;
};