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

#include "GLVertexArrayObject.h" //For loading attributes
#include "GLAttribute.h" //For loading attributes
#include "Programs/GLProgramDatabase.h" //For loading the program
#include "Programs/GLProgram.h" //For storing the program
#include "../Utils/Singleton.h" //For different global classes

/*---------------------------------------------
  GLMesh: Contains all the information needed
  to render objects.
---------------------------------------------*/

struct GLMeshData
{
	GLMeshData();
	GLMeshData(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, 
		GLuint numElements, GLenum drawType);
	~GLMeshData();
	
    GLenum drawType;
    std::vector<GLfloat> vertexBufferData;
    std::vector<GLushort> indexBufferData;
	GLuint numElements;
	std::string name;

	static GLMeshData* initialize(std::string filename);
};

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
	void setGLMeshData(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, GLuint numElements, GLenum drawType);
	void setGLMeshData(GLMeshData* meshData);
	GLMeshData* getGLMeshData();
	std::vector<GLfloat>& getVBOData();
	std::vector<GLushort>& getIBOData();
	int getNumElements();

	//name
	void setName(std::string name);
	std::string getName();

	//Visible
	void setVisible(bool visible);
	bool isVisible();

	//Render
    void Render();

private:

    void Generate();

	//Buffer object
    GLMeshData* meshData;
    GLVertexArrayObject* vertexArrayObject;
	GLProgram* program;
	GLuint vertexBuffer;
    GLuint indexBuffer;

	std::string name;
	bool visible;
};
