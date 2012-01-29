#pragma once

#include <vector>
#include <gl3w/gl3w.h>

#include "GLAttribute.h"

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
