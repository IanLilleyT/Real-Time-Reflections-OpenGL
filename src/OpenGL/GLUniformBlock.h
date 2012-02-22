#pragma once

#include <string> //For names

#include <glm/glm.hpp> //For math
#include <gl3w/gl3w.h> //For OpenGL commands

class GLUniformBlock
{
public:
	GLUniformBlock();
	~GLUniformBlock();

	//Other
	void createUBO();
	void bindToProgram(GLuint program);

	//Setters
	void setUBO(GLuint ubo);
	void setName(std::string name);
	void setBindingIndex(int index);
	void setData(void* data);
	void setSize(int size);

	//Getters
	GLuint getUBO();
	std::string getName();
	int getBindingIndex();
	int getSize();

private:
	GLuint ubo;
	std::string name;
	int bindingIndex;
	int size;
};