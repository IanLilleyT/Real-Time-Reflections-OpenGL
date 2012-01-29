#pragma once

#include "gl3w/gl3w.h"
#include "glm/glm.hpp"

#include "Face.h"
#include "Utils.h"

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

typedef std::pair<std::vector<GLfloat>,std::vector<GLushort>> VBOPair;

class Mesh
{
private:
    std::vector<Face*> faces;
	VBOPair vboPair;

public:
    Mesh();
    ~Mesh();

	//VBOPair
	VBOPair getVBOPair();

	//Faces
	std::vector<Face*> getFaces();

    //Static
    static Mesh* openOBJ(std::string filename);

private:
	void addFace(Face* face);
	void createVBOPair();
};
