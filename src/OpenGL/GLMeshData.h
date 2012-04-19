#pragma once

#include <vector>
#include <iostream>
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

#include <gl3w/gl3w.h>

#include "../Utils/BoundingBox.h"

class GLMeshData
{
public:
	GLMeshData();
	~GLMeshData();

	bool initialize(std::string filename);
	bool initialize(std::vector<GLfloat> vboData, std::vector<GLushort> iboData, 
		GLuint numElements, GLenum drawType);
	
    GLenum drawType;
    std::vector<GLfloat> vertexBufferData;
    std::vector<GLushort> indexBufferData;
	GLuint numElements;
	std::string name;
	BoundingBox* boundingBox;

private:
	bool initializeOBJ(std::string file);
};