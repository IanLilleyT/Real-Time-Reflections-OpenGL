#pragma once

#include <vector>
#include <string>

#include <gl3w/gl3w.h>
#include <glm/glm.hpp>

#include "GLBufferObject.h"
#include "GLVertexArrayObject.h"
#include "GLAttribute.h"
#include "GLState.h"
#include "GLProgramDatabase.h"
#include "GLProgram.h"
#include "../Singleton.h"

/*---------------------------------------------
  GLMesh: Contains all the information needed
  to render objects.

  Usage (call methods in this order):
  setVBOData, setProgramType, render
---------------------------------------------*/

class GLMesh
{
public:
    GLMesh();
    ~GLMesh();

	void setProgramType(std::string programName);
	std::string getProgramType();

	void setVBOData(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, GLenum drawType);
	std::vector<GLfloat>& getVBOData();
	std::vector<GLushort>& getIBOData();
    
    void Render();

private:

    void Generate();

    GLVertexArrayObject* vertexArrayObject;
    GLBufferObject* bufferObject;
    GLProgram* program;
};
