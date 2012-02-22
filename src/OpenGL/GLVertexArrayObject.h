#pragma once

#include <vector> //For storing Attrbiutes
#include <gl3w/gl3w.h> //For OpenGL commands

#include "GLAttribute.h" //For storing Attributes

class GLVertexArrayObject
{
public:
    GLVertexArrayObject();
	~GLVertexArrayObject();

    //Setters
    void setVertexArrayObject(GLuint vertexArrayObject);

    //Getters
    GLuint getVertexArrayObject();
    std::vector<GLAttribute*> getAttributes();
	void setAttributes(std::vector<GLAttribute*> attributes);

private:
    GLuint vertexArrayObject;
    std::vector<GLAttribute*> attributes;
};
