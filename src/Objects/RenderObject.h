#pragma once

#include <string> //For various properties

#include "Object.h" //Superclass
#include "../OpenGL/GLMesh.h" //For storing mesh
#include "../Material.h" //For storing material
#include "../OpenGL/GLState.h" //For updating model to camera matrix and material
#include "../MaterialDatabase.h" //For loading material based on string name
#include "../MeshDatabase.h" //For loading mesh based on string name
#include "../Utils/Singleton.h" //For accessing global classes

class RenderObject: public Object
{
public:
	//Class
	static std::string className;

	//Construct/Destruct
	RenderObject();
	virtual ~RenderObject();

	//Initialize
	virtual void initialize(TiXmlElement* element);
	void initialize(std::string name, std::string mesh, 
		std::string material, std::string program);

	//Type
	virtual std::string getClassname();

	//Update
	virtual void render();

	//Mesh
	void setMesh(std::string name);
	GLMesh* getMesh();

	//Material
	void setMaterial(std::string name);
	Material* getMaterial();

	//Program
	void setProgram(std::string program);
	GLProgram* getProgram();

protected:
	void initialize(std::string mesh, std::string material, std::string program);

	GLMesh* mesh;
	Material* material;
};