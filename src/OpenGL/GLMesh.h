#pragma once

//All for obj loading (probably a lot not needed)
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <istream>
#include <string>
#include <cstdlib>
#include <utility>
#include <limits.h>
#include <iterator>
#include <map>
#include <limits>
#include <cfloat>

#include <vector> //For setting VBO
#include <string> //For names

#include <gl3w/gl3w.h> //For GL commands

#include "GLBufferObject.h" //For storing VBO/IBO
#include "GLVertexArrayObject.h" //For loading attributes
#include "GLAttribute.h" //For loading attributes
#include "GLProgramDatabase.h" //For loading the program
#include "GLProgram.h" //For storing the program
#include "../Singleton.h" //For different global classes

/*---------------------------------------------
  GLMesh: Contains all the information needed
  to render objects.

  Usage (call methods in this order):
  setVBOData, setProgram, render
---------------------------------------------*/

class GLMesh
{
public:
    GLMesh();
    ~GLMesh();

	void initialize(std::string filename);

	//Program
	void setProgram(std::string programName);
	std::string getProgram();

	//VBO/IBO
	void setVBOData(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, GLuint numElements, GLenum drawType);
	std::vector<GLfloat>& getVBOData();
	std::vector<GLushort>& getIBOData();
    
	//Other
	void setVisible(bool visible);
	bool isVisible();
	int getNumElements();

	//name
	void setName(std::string name);
	std::string getName();

	//Render
    void Render();

private:

    void Generate();

    GLVertexArrayObject* vertexArrayObject;
    GLBufferObject* bufferObject;
    GLProgram* program;
	bool visible;
	std::string name;
};
