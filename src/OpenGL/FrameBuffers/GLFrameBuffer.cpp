#include "GLFrameBuffer.h"

GLFrameBuffer::GLFrameBuffer(){}
GLFrameBuffer::~GLFrameBuffer(){}

void GLFrameBuffer::initialize(){}
void GLFrameBuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}
void GLFrameBuffer::bindForReading(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture);
}