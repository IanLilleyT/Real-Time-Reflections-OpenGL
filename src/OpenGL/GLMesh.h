#pragma once

#include <vector>
#include <string>

#include <gl3w/gl3w.h>
#include <glm/glm.hpp>

#include "GLBufferObject.h"
#include "GLVertexArrayObject.h"
#include "GLAttribute.h"
#include "GLState.h"
#include "GLProgramHelper.h"
#include "GLProgram.h"
#include "../Singleton.h"

class GLMesh
{
public:
    GLMesh();
    ~GLMesh();

    //Setters
	void setProgram(std::string programName);

    //Getters
    GLProgram* getProgram();
    GLBufferObject* getBufferObject();
    GLVertexArrayObject* getVertexArrayObject();

    //Other
    void Render();

private:
    void Generate();

    GLVertexArrayObject* vertexArrayObject;
    GLBufferObject* bufferObject;
    GLProgram* program;
};
