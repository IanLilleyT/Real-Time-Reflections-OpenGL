#pragma once

#include <vector> //For setting VBO
#include <string> //For names

#include <gl3w/gl3w.h> //For GL commands

#include "Programs/GLProgramDatabase.h" //For loading the program
#include "Programs/GLProgram.h" //For storing the program
#include "../Utils/Singleton.h" //For different global classes
#include "GLMeshData.h"

/*---------------------------------------------
  GLMesh: Contains all the information needed
  to render objects.
---------------------------------------------*/

class GLMesh
{
public:
    GLMesh();
    ~GLMesh();

	//Initialize
	void initialize(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, GLuint numElements, GLenum drawType);
	void initialize(GLMeshData* meshData);
	GLMeshData* getGLMeshData();

	//Program
	void setProgram(std::string programName);
	GLProgram* getProgram();

	//Name
	void setName(std::string name);
	std::string getName();

	//Render
    void Render();

private:
	void Generate();

	//GLData
    GLMeshData* meshData;
	GLProgram* program;
	GLuint vertexBuffer;
    GLuint indexBuffer;
	GLuint vao;

	std::string name;
};
