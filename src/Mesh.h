#pragma once

#include <vector>
#include <string>

#include <gl3w/gl3w.h>
#include <glm/glm.hpp>

#include "Face.h"
#include "Utils.h"
#include "OpenGL/GLMesh.h"

/*---------------------------------------------
  Mesh: Contains render information for an object
  in the world
---------------------------------------------*/

class Mesh
{
friend class MeshDatabase;
public:
    Mesh();
    ~Mesh();

	//GLMesh
	void setGLMesh(GLMesh* glMesh);
	GLMesh* getGLMesh();

	//Program Type
	void setProgramType(std::string programType);
	std::string getProgramType();

	//Faces
	std::vector<Face*> getFaces();

private:
    std::vector<Face*> faces;
	GLMesh* glMesh;

	void addFace(Face* face);
	void createGLMesh();
};
