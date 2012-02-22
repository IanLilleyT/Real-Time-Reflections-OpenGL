#pragma once

#include <vector> //For storing uniform blocks
#include <string> //For program and attribute names

#include <gl3w/gl3w.h> //For OpenGL commands
#include <glm/glm.hpp> //For math
#include <glm/gtc/type_ptr.hpp> //For getting value_ptr of glm types

#include "GLState.h" //Used by subclasses for getting uniform data
#include "GLAttribute.h" //For constructing attributes
#include "GLVertexArrayObject.h" //Used by subclasses for creating VAO's
#include "GLUniformBlockHelper.h" //Used to find uniform blocks for binding
#include "GLUniformBlock.h" //Used for binding uniform blocks to program
#include "../Singleton.h" //Used with several global classes

class GLProgram
{
public:
    GLProgram();
    virtual ~GLProgram();
    
    //Setters
    void setProgram(GLuint program);
    void setName(std::string name);

    //Getters
    GLuint getProgram();
    std::string getName();
	GLVertexArrayObject* getVAO();

	//Other
	virtual void fillUniforms();
	virtual void createVAO();
	
protected:
	GLAttribute* getAttribute(std::string name, GLushort size, GLenum type);
	virtual void bindUniformBlocks();

	GLVertexArrayObject* vao;
    GLuint program;
    std::string name;
	std::vector<std::string> uniformBlocks;
};