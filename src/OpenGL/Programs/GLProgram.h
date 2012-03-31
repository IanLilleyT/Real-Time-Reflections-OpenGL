#pragma once

#include <vector> //For storing uniform blocks
#include <string> //For program and attribute names

#include <gl3w/gl3w.h> //For OpenGL commands
#include <glm/glm.hpp> //For math
#include <glm/gtc/type_ptr.hpp> //For getting value_ptr of glm types

#include "../GLState.h" //Used by subclasses for getting uniform data
#include "../GLAttribute.h" //For constructing attributes
#include "../GLUniformBlockHelper.h" //Used to find uniform blocks for binding
#include "../GLUniformBlock.h" //Used for binding uniform blocks to program
#include "../../Utils/Singleton.h" //Used with several global classes

class GLProgram
{
public:
    GLProgram();
    virtual ~GLProgram();
    
    //Setters
    void setProgram(GLuint program); //set from GLProgramDatabase
    void setName(std::string name);  //set from GLProgramDatabase

    //Getters
	std::vector<GLAttribute*>& getAttributes();
    GLuint getProgram();
    std::string getName();

	//Other
	virtual void fillUniforms();
	
protected:

	//Attributes
	virtual void createAttributes(); //set from subclasses
	virtual void addAttribute(std::string name, GLushort size, GLenum type);

	//Uniform Blocks
	virtual void bindUniformBlocks(); //set from subclasses
	virtual void bindUniformBlock(std::string uniformBlockName);
	
	std::vector<GLAttribute*> attributes;
    GLuint program;
    std::string name;
};