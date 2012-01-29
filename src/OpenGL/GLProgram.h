#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLState.h"
#include "GLAttribute.h"
#include "GLVertexArrayObject.h"
#include "GLUniformBlockHelper.h"
#include "GLUniformBlock.h"
#include "../Singleton.h"
#include "../Utils.h"

class GLProgram
{
public:
    GLProgram();
    virtual ~GLProgram();
    
    //Setters
    void setProgram(GLuint program);
    void setName(std::string name);

    //Getters
	GLAttribute* getAttribute(std::string name, GLushort size, GLenum type);
    GLuint getProgram();
    std::string getName();
	virtual GLVertexArrayObject* getVAO() = 0;
	virtual void fillUniforms();
	virtual void bindUniformBlocks();

protected:
    GLuint program;
    std::string name;
	std::vector<std::string> uniformBlocks;
};